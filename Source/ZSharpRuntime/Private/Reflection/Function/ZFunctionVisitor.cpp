// Copyright Zero Games. All Rights Reserved.

#include "Reflection/Function/ZFunctionVisitor.h"

#include "ALC/ZRedFrameScope.h"
#include "Emit/IZSharpFieldRegistry.h"
#include "Emit/ZSharpFieldRegistry.h"
#include "ZCall/ZCallBufferSlotEncoder.h"
#include "Reflection/Delegate/ZManagedDelegateProxy.h"

ZSharp::FZCallHandle ZSharp::FZFunctionVisitor::DelegateZCallHandle{};

ZSharp::EZCallErrorCode ZSharp::FZFunctionVisitor::InvokeUFunction(FZCallBuffer* buffer) const
{
	check(IsInGameThread());
	check(bAvailable);
	check(!bIsDelegate);

	// buffer can be null on static functions so we can't dereference.
	const UFunction* func = Function.Get();
	UObject* self;
	if (!bIsStatic)
	{
		self = TZCallBufferSlotEncoder<UObject*>::Decode((*buffer)[0]);
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

	return InternalInvokeUFunction(buffer, self, func);
}

ZSharp::EZCallErrorCode ZSharp::FZFunctionVisitor::InvokeUFunction(FZCallBuffer* buffer, const UFunction* finalFunction) const
{
	check(IsInGameThread());
	check(bAvailable);
	check(!bIsDelegate);

	// buffer can be null on static functions so we can't dereference.
	const UFunction* proto = Function.Get();
	UObject* self;
	if (!bIsStatic)
	{
		self = TZCallBufferSlotEncoder<UObject*>::Decode((*buffer)[0]);
		if (!self)
		{
			return EZCallErrorCode::BufferError;
		}
	}
	else
	{
		self = finalFunction->GetOuterUClass()->GetDefaultObject();
	}
	
	check(finalFunction->IsChildOf(proto));
	check(finalFunction->IsSignatureCompatibleWith(proto));

	check(self);

#if DO_CHECK
	const UClass* outerClass = finalFunction->GetOuterUClass();
	if (outerClass->HasAllClassFlags(CLASS_Interface))
	{
		check(self->GetClass()->ImplementsInterface(outerClass));
	}
	else
	{
		check(self->IsA(outerClass));
	}
#endif

	return InternalInvokeUFunction(buffer, self, finalFunction);
}

ZSharp::EZCallErrorCode ZSharp::FZFunctionVisitor::InvokeScriptDelegate(FZCallBuffer* buffer) const
{
	check(IsInGameThread());
	check(bAvailable);
	check(bIsDelegate);
	check(!bIsMulticastDelegate);
	check(!bIsStatic);

	checkf(!bIsRpc, TEXT("RPC is not supported on delegates."));

	FZCallBuffer& buf = *buffer;
	UFunction* func = Function.Get();
	FZSelfDescriptiveScriptDelegate& self = TZCallBufferSlotEncoder<FZSelfDescriptiveScriptDelegate>::Decode(buf[0]);
	check(self.GetDescriptor() == func);
	
	void* params = FMemory_Alloca_Aligned(func->ParmsSize, func->MinAlignment);

	FillParams(params, buffer);

	self.GetUnderlyingInstance()->ProcessDelegate<UObject>(params);

	CopyOut(buffer, params);
	
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

	checkf(!bIsRpc, TEXT("RPC is not supported on delegates."));

	FZCallBuffer& buf = *buffer;
	UFunction* func = Function.Get();
	FZSelfDescriptiveMulticastInlineScriptDelegate& self = TZCallBufferSlotEncoder<FZSelfDescriptiveMulticastInlineScriptDelegate>::Decode(buf[0]);
	check(self.GetDescriptor() == func);
	
	void* params = FMemory_Alloca_Aligned(func->ParmsSize, func->MinAlignment);

	FillParams(params, buffer);

	self.GetUnderlyingInstance()->ProcessMulticastDelegate<UObject>(params);

	CopyOut(buffer, params);

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

	checkf(!bIsRpc, TEXT("RPC is not supported on delegates."));

	FZCallBuffer& buf = *buffer;
	UFunction* func = Function.Get();
	FZSelfDescriptiveMulticastSparseScriptDelegate& self = TZCallBufferSlotEncoder<FZSelfDescriptiveMulticastSparseScriptDelegate>::Decode(buf[0]);
	check(self.GetDescriptor() == func);
	
	void* params = FMemory_Alloca_Aligned(func->ParmsSize, func->MinAlignment);

	FillParams(params, buffer);

	self.GetDelegatePtr()->ProcessMulticastDelegate<UObject>(params);

	CopyOut(buffer, params);

	params = nullptr;
	
	return EZCallErrorCode::Succeed;
}

ZSharp::EZCallErrorCode ZSharp::FZFunctionVisitor::InvokeZCall(UObject* object, FFrame& stack, RESULT_DECL) const
{
	check(IsInGameThread());
	check(bAvailable);
	check(!bIsDelegate);

	UFunction* currentFunction = stack.CurrentNativeFunction;
	const FZSharpFunction* zsfunction = FZSharpFieldRegistry::Get().GetFunction(currentFunction);
	check(zsfunction);
	check(currentFunction->IsChildOf(Function.Get()));
	check(currentFunction->IsSignatureCompatibleWith(Function.Get()));
	
	// IMPORTANT: Event if ALC is not available we can't return directly because we always need to balance the stack!!!
	IZMasterAssemblyLoadContext* alc = IZSharpClr::Get().GetMasterAlc();
	EZCallErrorCode res = EZCallErrorCode::AlcUnavailable;
	const int32 numSlots = (bIsStatic ? 0 : 1) + ParameterProperties.Num() + (ReturnProperty ? 1 : 0);
	FZRedFrameScope scope;
	{
		FZCallBuffer buffer;
		buffer.Slots = static_cast<FZCallBufferSlot*>(FMemory_Alloca(numSlots * sizeof(FZCallBufferSlot)));
		buffer.NumSlots = numSlots;

		FMemory::Memzero(buffer.Slots, numSlots * sizeof(FZCallBufferSlot));
		for (int32 i = 0; i < numSlots; ++i)
		{
			buffer[i].Type = EZCallBufferSlotType::Uninitialized;
		}

		const UFunction* func = Function.Get();

		// Blueprint doesn't create new stack frame when it calls native function (and we are native)
		// which results in stack.Node points to the caller and stack.CurrentNativeFunction points to us.
		const bool fromBlueprint = stack.Node != stack.CurrentNativeFunction;

		// For native call, params store directly in stack.Locals. (caller processes params)
		// For blueprint call, we need to manually alloc memory for params and process bytecode to load them. (callee processes params)
		void* params = fromBlueprint ? FMemory_Alloca_Aligned(func->ParmsSize, func->MinAlignment) : stack.Locals;

		// For native call, out param link is stack.OutParms. (caller processes params)
		// For blueprint call, we need to process bytecode to setup out param link. (callee processes params) (@see: ProcessScriptFunction() in ScriptCore.cpp)
		FOutParmRec* outParmLink = fromBlueprint ? nullptr : stack.OutParms;
		
		// If called from blueprint, we need to fill params.
		if (fromBlueprint)
		{
			FOutParmRec** lastOut = &outParmLink;
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

		if (alc)
		{
			bool validatePassed = true;
			if (!zsfunction->ValidateZCallName.IsEmpty())
			{
				check(func->HasAllFunctionFlags(FUNC_Net | FUNC_NetValidate));
				
				const int32 validateNumSlots = numSlots + 1;
				FZCallBuffer validateBuffer;
				validateBuffer.Slots = static_cast<FZCallBufferSlot*>(FMemory_Alloca(validateNumSlots * sizeof(FZCallBufferSlot)));
				validateBuffer.NumSlots = validateNumSlots;
				// ZCallBufferSlot is POD type so we can use memcpy.
				FMemory::Memcpy(validateBuffer.Slots, buffer.Slots, (validateNumSlots - 1) * sizeof(FZCallBufferSlot));
				validateBuffer[-1] = FZCallBufferSlot::FromBool(false);

				alc->ZCall(zsfunction->GetValidateZCallHandle(), &validateBuffer);
				validatePassed = validateBuffer[-1].ReadBool();
			}

			if (validatePassed)
			{
				res = alc->ZCall(zsfunction->GetZCallHandle(), &buffer);
			}
			else
			{
				RPC_ValidateFailed(*FString::Printf(TEXT("%s_Validate"), *func->GetName()));
			}
		}

		// Copy out params and destroy them if called from blueprint.
		for (int32 i = 0; i < ParameterProperties.Num(); ++i)
		{
			const TUniquePtr<IZPropertyVisitor>& visitor = ParameterProperties[i];
			if (OutParamIndices.Contains(i))
			{
				FOutParmRec* out = outParmLink;
				// This must succeed, otherwise let it crash by null pointer.
				while (out->Property != visitor->GetUnderlyingProperty())
				{
					out = out->NextOutParm;
				}
				visitor->SetValue(out->PropAddr, buffer[bIsStatic ? i : i + 1]);
			}
			
			if (fromBlueprint)
			{
				visitor->DestructValue_InContainer(params);
			}
		}

		if (ReturnProperty)
		{
			ReturnProperty->SetValue(RESULT_PARAM, buffer[-1]);
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

	checkf(!bIsRpc, TEXT("RPC is not supported on delegates."));

	check(object);
	check(object->Implements<UZManagedDelegateProxy>());

	IZMasterAssemblyLoadContext* alc = IZSharpClr::Get().GetMasterAlc();
	if (!alc)
	{
		return EZCallErrorCode::AlcUnavailable;
	}

	IZManagedDelegateProxy* proxy = CastChecked<IZManagedDelegateProxy>(object);
	TOptional<FZGCHandle> state = proxy->ManagedDelegateProxy_GetState();
	const int32 numSlots = 1 + ParameterProperties.Num() + (state ? 1 : 0) + (ReturnProperty ? 1 : 0);
	EZCallErrorCode res;
	FZRedFrameScope scope;
	{
		FZCallBuffer buffer;
		buffer.Slots = static_cast<FZCallBufferSlot*>(FMemory_Alloca(numSlots * sizeof(FZCallBufferSlot)));
		buffer.NumSlots = numSlots;

		FMemory::Memzero(buffer.Slots, numSlots * sizeof(FZCallBufferSlot));
		for (int32 i = 0; i < numSlots; ++i)
		{
			buffer[i].Type = EZCallBufferSlotType::Uninitialized;
		}
		
		TZCallBufferSlotEncoder<FZGCHandle>::Encode(proxy->ManagedDelegateProxy_GetDelegate(), buffer[0]);

		for (int32 i = 0; i < ParameterProperties.Num(); ++i)
		{
			const TUniquePtr<IZPropertyVisitor>& visitor = ParameterProperties[i];
			// We need to set Type for all slots, not only in params.
			visitor->GetValue_InContainer(params, buffer[i + 1], 0);
		}

		if (state)
		{
			TZCallBufferSlotEncoder<FZGCHandle>::Encode(*state, buffer[ReturnProperty ? -2 : -1]);
		}

		if (ReturnProperty)
		{
			// We need to set Type for all slots, not only in params.
			ReturnProperty->GetValue_InContainer(params, buffer[-1], 0);
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

ZSharp::EZCallErrorCode ZSharp::FZFunctionVisitor::InternalInvokeUFunction(FZCallBuffer* buffer, UObject* object, const UFunction* function) const
{
	void* params = FMemory_Alloca_Aligned(function->ParmsSize, function->MinAlignment);

	FillParams(params, buffer);

	object->ProcessEvent(const_cast<UFunction*>(function), params);

	CopyOut(buffer, params);

	params = nullptr;

	return EZCallErrorCode::Succeed;
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
    		DelegateZCallHandle = alc->GetZCallHandle("nd://");
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
	bIsRpc = function->HasAllFunctionFlags(FUNC_Net);
	
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

void ZSharp::FZFunctionVisitor::FillParams(void* params, const FZCallBuffer* buffer) const
{
	for (int32 i = 0; i < ParameterProperties.Num(); ++i)
	{
		const TUniquePtr<IZPropertyVisitor>& visitor = ParameterProperties[i];
		visitor->InitializeValue_InContainer(params);
		if (InParamIndices.Contains(i))
		{
			visitor->SetValue_InContainer(params, (*buffer)[bIsStatic ? i : i + 1], 0);
		}
	}

	if (ReturnProperty)
	{
		ReturnProperty->InitializeValue_InContainer(params);
	}
}

void ZSharp::FZFunctionVisitor::CopyOut(FZCallBuffer* buffer, void* params) const
{
	for (int32 i = 0; i < ParameterProperties.Num(); ++i)
	{
		const TUniquePtr<IZPropertyVisitor>& visitor = ParameterProperties[i];
		if (OutParamIndices.Contains(i))
		{
			visitor->GetValue_InContainer(params, (*buffer)[bIsStatic ? i : i + 1], 0);
		}
		visitor->DestructValue_InContainer(params);
	}

	if (ReturnProperty)
	{
		ReturnProperty->GetValue_InContainer(params, (*buffer)[-1], 0);
		ReturnProperty->DestructValue_InContainer(params);
	}
}


