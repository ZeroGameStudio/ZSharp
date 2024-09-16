// Copyright Zero Games. All Rights Reserved.


#include "Reflection/Function/ZFunctionVisitor.h"

#include "Emit/IZSharpFieldRegistry.h"
#include "Emit/ZSharpFieldRegistry.h"
#include "ZCall/ZCallBufferSlotEncoder.h"
#include "ZCall/ZManagedDelegateProxyInterface.h"

ZSharp::FZCallHandle ZSharp::FZFunctionVisitor::DelegateZCallHandle{};

ZSharp::EZCallErrorCode ZSharp::FZFunctionVisitor::InvokeUFunction(FZCallBuffer* buffer) const
{
	check(IsInGameThread());
	check(bAvailable);
	check(!bIsDelegate);

	checkf(!bIsRpc, TEXT("RPC is not supported yet."));

	FZCallBuffer& buf = *buffer;
	UFunction* func = Function.Get();
	UObject* self;
	if (!bIsStatic)
	{
		self = TZCallBufferSlotEncoder<UObject*>::Decode(buf[0]);
		if (!self)
		{
			return EZCallErrorCode::BufferError;
		}
		const UFunction* proto = func;
		func = self->FindFunctionChecked(func->GetFName()); // Instance function may be polymorphism
		check(func->IsChildOf(proto));
		check(func->IsSignatureCompatibleWith(proto));
	}
	else
	{
		self = func->GetOuterUClass()->GetDefaultObject();
	}

	check(self);
	check(self->IsA(func->GetOuterUClass()));
	
	void* params = FMemory_Alloca_Aligned(func->ParmsSize, func->MinAlignment);

	FillParams(params, buf);

	self->ProcessEvent(func, params);

	CopyOut(buf, params);
	
	params = nullptr;

	return EZCallErrorCode::Succeed;
}

ZSharp::EZCallErrorCode ZSharp::FZFunctionVisitor::InvokeScriptDelegate(FZCallBuffer* buffer) const
{
	check(IsInGameThread());
	check(bAvailable);
	check(bIsDelegate);
	check(!bIsMulticastDelegate);
	check(!bIsStatic);

	checkf(!bIsRpc, TEXT("RPC is not supported yet."));

	FZCallBuffer& buf = *buffer;
	UFunction* func = Function.Get();
	FZSelfDescriptiveScriptDelegate& self = TZCallBufferSlotEncoder<FZSelfDescriptiveScriptDelegate>::Decode(buf[0]);
	check(self.GetDescriptor() == func);
	
	void* params = FMemory_Alloca_Aligned(func->ParmsSize, func->MinAlignment);

	FillParams(params, buf);

	self.GetUnderlyingInstance()->ProcessDelegate<UObject>(params);

	CopyOut(buf, params);
	
	params = nullptr;

	return EZCallErrorCode::Succeed;
}

ZSharp::EZCallErrorCode ZSharp::FZFunctionVisitor::InvokeMulticastInlineScriptDelegate(FZCallBuffer* buffer) const
{
	check(IsInGameThread());
	check(bAvailable);
	check(bIsDelegate);
	check(bIsMulticastDelegate);
	check(!ReturnProperty);
	check(!bIsStatic);

	checkf(!bIsRpc, TEXT("RPC is not supported yet."));

	FZCallBuffer& buf = *buffer;
	UFunction* func = Function.Get();
	FZSelfDescriptiveMulticastInlineScriptDelegate& self = TZCallBufferSlotEncoder<FZSelfDescriptiveMulticastInlineScriptDelegate>::Decode(buf[0]);
	check(self.GetDescriptor() == func);
	
	void* params = FMemory_Alloca_Aligned(func->ParmsSize, func->MinAlignment);

	FillParams(params, buf);

	self.GetUnderlyingInstance()->ProcessMulticastDelegate<UObject>(params);

	CopyOut(buf, params);

	params = nullptr;
	
	return EZCallErrorCode::Succeed;
}

ZSharp::EZCallErrorCode ZSharp::FZFunctionVisitor::InvokeMulticastSparseScriptDelegate(FZCallBuffer* buffer) const
{
	check(IsInGameThread());
	check(bAvailable);
	check(bIsDelegate);
	check(bIsMulticastDelegate);
	check(!ReturnProperty);
	check(!bIsStatic);

	checkf(!bIsRpc, TEXT("RPC is not supported yet."));

	FZCallBuffer& buf = *buffer;
	UFunction* func = Function.Get();
	FZSelfDescriptiveMulticastSparseScriptDelegate& self = TZCallBufferSlotEncoder<FZSelfDescriptiveMulticastSparseScriptDelegate>::Decode(buf[0]);
	check(self.GetDescriptor() == func);
	
	void* params = FMemory_Alloca_Aligned(func->ParmsSize, func->MinAlignment);

	FillParams(params, buf);

	self.GetDelegatePtr()->ProcessMulticastDelegate<UObject>(params);

	CopyOut(buf, params);

	params = nullptr;
	
	return EZCallErrorCode::Succeed;
}

ZSharp::EZCallErrorCode ZSharp::FZFunctionVisitor::InvokeZCall(UObject* object, FFrame& stack, RESULT_DECL) const
{
	check(IsInGameThread());
	check(bAvailable);
	check(!bIsDelegate);

	checkf(!bIsRpc, TEXT("RPC is not supported yet."));

	UFunction* currentFunction = stack.CurrentNativeFunction;
	const FZSharpFunction* zsfunction = FZSharpFieldRegistry::Get().GetFunction(currentFunction);
	check(zsfunction);
	check(currentFunction->IsChildOf(Function.Get()));
	check(currentFunction->IsSignatureCompatibleWith(Function.Get()));

	IZMasterAssemblyLoadContext* alc = IZSharpClr::Get().GetMasterAlc();
	if (!alc)
	{
		return EZCallErrorCode::AlcUnavailable;
	}

	const int32 numSlots = (bIsStatic ? 0 : 1) + ParameterProperties.Num() + (ReturnProperty ? 1 : 0);
	EZCallErrorCode res;
	alc->PrepareForZCall();
	{
		FZCallBuffer buffer;
		buffer.Slots = static_cast<FZCallBufferSlot*>(FMemory_Alloca(numSlots * sizeof(FZCallBufferSlot)));
		buffer.NumSlots = numSlots;

		FMemory::Memzero(buffer.Slots, numSlots * sizeof(FZCallBufferSlot));
		for (int32 i = 0; i < numSlots; ++i)
		{
			buffer.Slots[i].Type = EZCallBufferSlotType::Uninitialized;
		}

		UFunction* func = Function.Get();

		// Blueprint doesn't create new stack frame when it calls native function (and we are native)
		// which results stack.Node points to the caller and stack.CurrentNativeFunction points to us.
		const bool fromBlueprint = stack.Node != stack.CurrentNativeFunction;

		// For native call, params store directly in stack.Locals. (caller processes params)
		// For blueprint call, we need to manually alloc memory for params and process bytecode to load them. (callee processes params)
		void* params = fromBlueprint ? FMemory_Alloca_Aligned(func->ParmsSize, func->MinAlignment) : stack.Locals;

		// For native call, out param link is stack.OutParms. (caller processes params)
		// For blueprint call, we need to process bytecode to setup out param link. (callee processes params) (@see: ProcessScriptFunction() in ScriptCore.cpp)
		FOutParmRec* outPramLink = fromBlueprint ? nullptr : stack.OutParms;
		
		// If called from blueprint, we need to fill params.
		if (fromBlueprint)
		{
			FOutParmRec** lastOut = &outPramLink;
			for (int32 i = 0; i < ParameterProperties.Num(); ++i)
			{
				const TUniquePtr<IZPropertyVisitor>& visitor = ParameterProperties[i];
				// We need to set Type for all slots, not only in params, so we initialize all properties.
				visitor->InitializeValue_InContainer(params);

				stack.MostRecentPropertyAddress = nullptr;
				stack.MostRecentPropertyContainer = nullptr;

				stack.Step(object, visitor->ContainerPtrToValuePtr(params, 0));

				if (OutParamIndices.Contains(i))
				{
					CA_SUPPRESS(6263)
					const auto out = static_cast<FOutParmRec*>(FMemory_Alloca(sizeof(FOutParmRec)));
					check(stack.MostRecentPropertyAddress);
					out->PropAddr = stack.MostRecentPropertyAddress;
					out->Property = const_cast<FProperty*>(visitor->GetUnderlyingProperty());

					if (*lastOut)
					{
						(*lastOut)->NextOutParm = out;
						lastOut = &(*lastOut)->NextOutParm;
					}
					else
					{
						*lastOut = out;
					}
				}
			}

			// Here the bytecode should reach EX_EndFunctionParms and we just skip it.
			check(*stack.Code == EX_EndFunctionParms);
			stack.SkipCode(1);
		}
		
		// Now we can fill ZCallBuffer.
		if (!bIsStatic)
		{
			TZCallBufferSlotEncoder<UObject*>::Encode(object, buffer[0]);
		}

		for (int32 i = 0; i < ParameterProperties.Num(); ++i)
		{
			const TUniquePtr<IZPropertyVisitor>& visitor = ParameterProperties[i];
			// We need to set Type for all slots, not only in params.
			visitor->GetValue_InContainer(params, buffer[bIsStatic ? i : i + 1], 0);
		}
		
		if (ReturnProperty)
		{
			// We need to set Type for all slots, not only in params.
			ReturnProperty->GetValue(RESULT_PARAM, buffer[-1]);
		}

		res = alc->ZCall(zsfunction->GetZCallHandle(), &buffer);

		// Copy out params and destroy them if called from blueprint.
		for (int32 i = 0; i < ParameterProperties.Num(); ++i)
		{
			const TUniquePtr<IZPropertyVisitor>& visitor = ParameterProperties[i];
			if (fromBlueprint && OutParamIndices.Contains(i))
			{
				FOutParmRec* out = outPramLink;
				// This must succeed, otherwise let it crash by null pointer.
				while (out->Property != visitor->GetUnderlyingProperty())
				{
					out = out->NextOutParm;
				}
				visitor->SetValue(out->PropAddr, buffer[bIsStatic ? i : i + 1]);
			}
			visitor->DestructValue_InContainer(params);
		}

		if (ReturnProperty)
		{
			ReturnProperty->SetValue(RESULT_PARAM, buffer[-1]);
			if (fromBlueprint)
			{
				ReturnProperty->DestructValue_InContainer(params);
			}
		}
	}

	return res;
}

ZSharp::EZCallErrorCode ZSharp::FZFunctionVisitor::InvokeZCall(UObject* object, void* params) const
{
	check(IsInGameThread());
	check(bAvailable);
	check(bIsDelegate);
	check(!bIsStatic);

	checkf(!bIsRpc, TEXT("RPC is not supported yet."));

	check(object);
	check(object->Implements<UZManagedDelegateProxyInterface>());

	IZMasterAssemblyLoadContext* alc = IZSharpClr::Get().GetMasterAlc();
	if (!alc)
	{
		return EZCallErrorCode::AlcUnavailable;
	}

	const int32 numSlots = 1 + ParameterProperties.Num() + (ReturnProperty ? 1 : 0);
	EZCallErrorCode res;
	alc->PrepareForZCall();
	{
		FZCallBuffer buffer;
		buffer.Slots = static_cast<FZCallBufferSlot*>(FMemory_Alloca(numSlots * sizeof(FZCallBufferSlot)));
		buffer.NumSlots = numSlots;

		FMemory::Memzero(buffer.Slots, numSlots * sizeof(FZCallBufferSlot));
		for (int32 i = 0; i < numSlots; ++i)
		{
			buffer.Slots[i].Type = EZCallBufferSlotType::Uninitialized;
		}

		TZCallBufferSlotEncoder<FZGCHandle>::Encode(CastChecked<IZManagedDelegateProxyInterface>(object)->ManagedDelegateProxy_GetDelegate(), buffer[0]);

		for (int32 i = 0; i < ParameterProperties.Num(); ++i)
		{
			const TUniquePtr<IZPropertyVisitor>& visitor = ParameterProperties[i];
			visitor->InitializeValue_InContainer(params);
			// We need to set Type for all slots, not only in params.
			visitor->GetValue_InContainer(params, buffer[i + 1], 0);
		}

		if (ReturnProperty)
		{
			// We need to set Type for all slots, not only in params.
			ReturnProperty->InitializeValue_InContainer(params);
		}
		
		res = alc->ZCall(GetDelegateZCallHandle(), &buffer);

		for (int32 i = 0; i < ParameterProperties.Num(); ++i)
		{
			const TUniquePtr<IZPropertyVisitor>& visitor = ParameterProperties[i];
			if (OutParamIndices.Contains(i))
			{
				visitor->SetValue_InContainer(params, buffer[i + 1], 0);
			}
			visitor->DestructValue_InContainer(params);
		}

		if (ReturnProperty)
		{
			ReturnProperty->SetValue_InContainer(params, buffer[-1], 0);
			ReturnProperty->DestructValue_InContainer(params);
		}
	}

	return res;
}

void ZSharp::FZFunctionVisitor::StaticClearAlcSensitiveStates()
{
	DelegateZCallHandle = {};
}

ZSharp::FZCallHandle ZSharp::FZFunctionVisitor::GetDelegateZCallHandle()
{
	if (!DelegateZCallHandle)
    {
    	if (IZMasterAssemblyLoadContext* alc = IZSharpClr::Get().GetMasterAlc())
    	{
    		DelegateZCallHandle = alc->GetZCallHandle("d://");
    	}
    }
    
    check(DelegateZCallHandle);
    return DelegateZCallHandle;
}

ZSharp::FZFunctionVisitor::FZFunctionVisitor(UFunction* function)
	: bAvailable(false)
	, bIsStatic(false)
	, bIsDelegate(false)
	, bIsMulticastDelegate(false)
	, bIsRpc(false)
{
	Function = function;
	if (!Function.IsValid(true))
	{
		return;
	}

	check(!function->GetSuperFunction());

	bIsStatic = function->HasAllFunctionFlags(FUNC_Static);
	bIsDelegate = function->HasAllFunctionFlags(FUNC_Delegate);
	bIsMulticastDelegate = function->HasAllFunctionFlags(FUNC_MulticastDelegate);
	bIsRpc = function->HasAnyFunctionFlags(FUNC_NetServer | FUNC_NetClient | FUNC_NetMulticast);
	
	ParameterProperties.Empty();
	ReturnProperty.Reset();
	OutParamIndices.Empty();
	
	for (TFieldIterator<FProperty> it(Function.Get()); it && it->HasAllPropertyFlags(CPF_Parm); ++it)
	{
		FProperty* prop = *it;
		TUniquePtr<IZPropertyVisitor> visitor = IZPropertyVisitor::Create(prop);
		if (!visitor)
		{
			return;
		}

		if (prop->HasAllPropertyFlags(CPF_ReturnParm))
		{
			ReturnProperty = MoveTemp(visitor);
			continue;
		}

		const int32 index = ParameterProperties.Emplace(MoveTemp(visitor));
		if (prop->HasAllPropertyFlags(CPF_OutParm) && !prop->HasAnyPropertyFlags(CPF_ConstParm))
		{
			if (prop->HasAllPropertyFlags(CPF_ReferenceParm))
			{
				InParamIndices.Emplace(index);
			}
			OutParamIndices.Emplace(index);
		}
		else
		{
			InParamIndices.Emplace(index);
		}
	}

	bAvailable = true;
}

void ZSharp::FZFunctionVisitor::FillParams(void* params, const FZCallBuffer& buf) const
{
	for (int32 i = 0; i < ParameterProperties.Num(); ++i)
	{
		const TUniquePtr<IZPropertyVisitor>& visitor = ParameterProperties[i];
		visitor->InitializeValue_InContainer(params);
		if (InParamIndices.Contains(i))
		{
			visitor->SetValue_InContainer(params, buf[bIsStatic ? i : i + 1], 0);
		}
	}
}

void ZSharp::FZFunctionVisitor::CopyOut(FZCallBuffer& buf, void* params) const
{
	for (int32 i = 0; i < ParameterProperties.Num(); ++i)
	{
		const TUniquePtr<IZPropertyVisitor>& visitor = ParameterProperties[i];
		if (OutParamIndices.Contains(i))
		{
			visitor->GetValue_InContainer(params, buf[bIsStatic ? i : i + 1], 0);
		}
		visitor->DestructValue_InContainer(params);
	}

	if (ReturnProperty)
	{
		ReturnProperty->GetValue_InContainer(params, buf[-1], 0);
		ReturnProperty->DestructValue_InContainer(params);
	}
}


