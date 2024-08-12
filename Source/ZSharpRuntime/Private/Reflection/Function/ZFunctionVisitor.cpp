// Copyright Zero Games. All Rights Reserved.


#include "Reflection/Function/ZFunctionVisitor.h"

#include "ZCall/ZCallBufferSlotEncoder.h"
#include "ZCall/ZManagedDelegateProxyInterface.h"

ZSharp::EZCallErrorCode ZSharp::FZFunctionVisitor::InvokeUFunction(FZCallBuffer* buffer) const
{
	check(bAvailable);
	check(!bIsDelegate);

	checkf(!bIsRpc, TEXT("RPC is not supported yet."));

	FZCallBuffer& buf = *buffer;
	UFunction* func = Function.Get(); // @FIXME: Dynamic function may get GCed.
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

	for (int32 i = 0; i < ParameterProperties.Num(); ++i)
	{
		const TUniquePtr<IZPropertyVisitor>& visitor = ParameterProperties[i];
		visitor->InitializeValue_InContainer(params);
		visitor->SetValue_InContainer(params, buf[bIsStatic ? i : i + 1], 0);
	}

	if (ReturnProperty)
	{
		ReturnProperty->InitializeValue_InContainer(params);
	}

	self->ProcessEvent(func, params);

	for (const auto index : OutParamIndices)
	{
		const TUniquePtr<IZPropertyVisitor>& visitor = ParameterProperties[index];
		visitor->GetValue_InContainer(params, buf[bIsStatic ? index : index + 1], 0);
	}

	if (ReturnProperty)
	{
		ReturnProperty->GetValue_InContainer(params, buf[-1], 0);
	}

	return EZCallErrorCode::Succeed;
}

ZSharp::EZCallErrorCode ZSharp::FZFunctionVisitor::InvokeScriptDelegate(FZCallBuffer* buffer) const
{
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

	for (int32 i = 0; i < ParameterProperties.Num(); ++i)
	{
		const TUniquePtr<IZPropertyVisitor>& visitor = ParameterProperties[i];
		visitor->InitializeValue_InContainer(params);
		visitor->SetValue_InContainer(params, buf[i + 1], 0);
	}
	
	if (ReturnProperty)
	{
		ReturnProperty->InitializeValue_InContainer(params);
	}

	self.GetUnderlyingInstance()->ProcessDelegate<UObject>(params);

	for (const auto index : OutParamIndices)
	{
		const TUniquePtr<IZPropertyVisitor>& visitor = ParameterProperties[index];
		visitor->GetValue_InContainer(params, buf[index + 1], 0);
	}

	if (ReturnProperty)
	{
		ReturnProperty->GetValue_InContainer(params, buf[-1], 0);
	}

	return EZCallErrorCode::Succeed;
}

ZSharp::EZCallErrorCode ZSharp::FZFunctionVisitor::InvokeMulticastInlineScriptDelegate(FZCallBuffer* buffer) const
{
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

	for (int32 i = 0; i < ParameterProperties.Num(); ++i)
	{
		const TUniquePtr<IZPropertyVisitor>& visitor = ParameterProperties[i];
		visitor->InitializeValue_InContainer(params);
		visitor->SetValue_InContainer(params, buf[i + 1], 0);
	}

	self.GetUnderlyingInstance()->ProcessMulticastDelegate<UObject>(params);

	for (const auto index : OutParamIndices)
	{
		const TUniquePtr<IZPropertyVisitor>& visitor = ParameterProperties[index];
		visitor->GetValue_InContainer(params, buf[index + 1], 0);
	}
	
	return EZCallErrorCode::Succeed;
}

ZSharp::EZCallErrorCode ZSharp::FZFunctionVisitor::InvokeMulticastSparseScriptDelegate(FZCallBuffer* buffer) const
{
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

	for (int32 i = 0; i < ParameterProperties.Num(); ++i)
	{
		const TUniquePtr<IZPropertyVisitor>& visitor = ParameterProperties[i];
		visitor->InitializeValue_InContainer(params);
		visitor->SetValue_InContainer(params, buf[i + 1], 0);
	}

	self.GetDelegatePtr()->ProcessMulticastDelegate<UObject>(params);

	for (const auto index : OutParamIndices)
	{
		const TUniquePtr<IZPropertyVisitor>& visitor = ParameterProperties[index];
		visitor->GetValue_InContainer(params, buf[index + 1], 0);
	}
	
	return EZCallErrorCode::Succeed;
}

ZSharp::EZCallErrorCode ZSharp::FZFunctionVisitor::InvokeZCall(FZCallHandle handle, UObject* object, void* params) const
{
	check(bAvailable);

	checkf(!bIsRpc, TEXT("RPC is not supported yet."));

	IZMasterAssemblyLoadContext* alc = IZSharpClr::Get().GetMasterAlc();
	if (!alc)
	{
		return EZCallErrorCode::AlcUnavailable;
	}

	int32 numSlots = ParameterProperties.Num() + (ReturnProperty ? 1 : 0);
	UObject* self;
	if (bIsDelegate)
	{
		checkSlow(handle == alc->GetZCallHandle("d://"));
		check(!bIsStatic);
		check(object);
		check(object->Implements<UZManagedDelegateProxyInterface>());
	}
	else if (bIsStatic)
	{
		checkNoEntry(); // @TODO: Dynamic Class
	}
	else
	{
		checkNoEntry(); // @TODO: Dynamic Class
	}
	
	if (self = object; self)
	{
		++numSlots;
	}

	check(self || bIsStatic);
	
	EZCallErrorCode res;
	alc->PrepareForZCall();
	{
		FZCallBuffer buffer;
		buffer.Slots = (FZCallBufferSlot*)FMemory_Alloca(numSlots * sizeof(FZCallBufferSlot));
		buffer.NumSlots = numSlots;

		FMemory::Memzero(buffer.Slots, numSlots * sizeof(FZCallBufferSlot));
		for (int32 i = 0; i < numSlots; ++i)
		{
			buffer.Slots[i].Type = EZCallBufferSlotType::Uninitialized;
		}

		if (bIsDelegate)
		{
			TZCallBufferSlotEncoder<FZGCHandle>::Encode(CastChecked<IZManagedDelegateProxyInterface>(self)->ManagedDelegateProxy_GetDelegate(), buffer[0]);
		}
		else if (!bIsStatic)
		{
			TZCallBufferSlotEncoder<UObject*>::Encode(self, buffer[0]);
		}

		for (int32 i = 0; i < ParameterProperties.Num(); ++i)
		{
			const TUniquePtr<IZPropertyVisitor>& visitor = ParameterProperties[i];
			visitor->GetValue_InContainer(params, buffer[bIsStatic ? i : i + 1], 0);
		}

		if (ReturnProperty)
		{
			ReturnProperty->GetValue_InContainer(params, buffer[-1], 0);
		}
		
		res = alc->ZCall(handle, &buffer);
		
		for (const auto index : OutParamIndices)
		{
			const TUniquePtr<IZPropertyVisitor>& visitor = ParameterProperties[index];
			visitor->SetValue_InContainer(params, buffer[bIsStatic ? index : index + 1], 0);
		}

		if (ReturnProperty)
		{
			ReturnProperty->SetValue_InContainer(params, buffer[-1], 0);
		}
	}

	return res;
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
			OutParamIndices.Emplace(index);
		}
	}

	bAvailable = true;
}


