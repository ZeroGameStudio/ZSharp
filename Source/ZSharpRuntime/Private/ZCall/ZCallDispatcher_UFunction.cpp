// Copyright Zero Games. All Rights Reserved.


#include "ZCallDispatcher_UFunction.h"

#include "ZCall/ZCallBuffer.h"
#include "ZCall/ZCallBufferSlotEncoder.h"

int32 ZSharp::FZCallDispatcher_UFunction::Dispatch(FZCallBuffer* buffer) const
{
	if (!Function.IsValid(true))
	{
		Function = LoadObject<UFunction>(nullptr, *Path);
		if (!Function.IsValid(true))
		{
			return 1;
		}
		
		bAvailable = InvalidateCache();
	}

	if (!bAvailable)
	{
		return 2;
	}

	FZCallBuffer& buf = *buffer;
	UFunction* func = Function.Get();
	UObject* self = nullptr;
	if (!func->HasAllFunctionFlags(FUNC_Static))
	{
		self = TZCallBufferSlotEncoder<UObject*>::Decode(buf[0]);
		if (!self)
		{
			return 3;
		}
	}

	void* params = FMemory::Malloc(func->ParmsSize, func->MinAlignment);
	ON_SCOPE_EXIT { FMemory::Free(params); };

	for (int32 i = 0; i < ParameterProperties.Num(); ++i)
	{
		const TUniquePtr<IZPropertyVisitor>& visitor = ParameterProperties[i];
		visitor->InitializeValue_InContainer(params);
		visitor->SetValue_InContainer(params, buf[self ? i + 1 : i]);
	}

	(self ? self : func->GetOuterUClass()->GetDefaultObject())->ProcessEvent(func, params);

	for (const auto index : OutParamIndices)
	{
		const TUniquePtr<IZPropertyVisitor>& visitor = ParameterProperties[index];
		visitor->GetValue_InContainer(params, buf[self ? index + 1 : index]);
	}

	if (ReturnProperty)
	{
		ReturnProperty->GetValue_InContainer(params, buf[-1]);
	}
	
	return 0;
}

bool ZSharp::FZCallDispatcher_UFunction::InvalidateCache() const
{
	for (TFieldIterator<FProperty> it(Function.Get()); it && it->HasAllPropertyFlags(CPF_Parm); ++it)
	{
		FProperty* prop = *it;
		TUniquePtr<IZPropertyVisitor> visitor = IZPropertyVisitor::Create(prop);
		if (!visitor)
		{
			return false;
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

	return true;
}


