// Copyright Zero Games. All Rights Reserved.


#include "ZCallDispatcher_UFunction.h"

#include "ZCall/ZCallBuffer.h"
#include "ZCall/ZCallBufferSlotEncoder.h"

int32 ZSharp::FZCallDispatcher_UFunction::Dispatch(FZCallBuffer* buffer) const
{
	if (!Function.IsValid(true))
	{
		if (bAvailable = InvalidateCache(); !bAvailable)
		{
			return 1;
		}
	}

	if (!bAvailable)
	{
		return 2;
	}

	FZCallBuffer& buf = *buffer;
	UFunction* func = Function.Get();
	const bool staticFunc = func->HasAllFunctionFlags(FUNC_Static);
	UObject* self;
	if (!staticFunc)
	{
		self = TZCallBufferSlotEncoder<UObject*>::Decode(buf[0]);
		if (!self)
		{
			return 3;
		}
		const UFunction* proto = func;
		func = self->GetClass()->FindFunctionByName(func->GetFName()); // Instance function may be polymorphism
		check(func->IsChildOf(proto));
		check(func->IsSignatureCompatibleWith(proto));
	}
	else
	{
		self = func->GetOuterUClass()->GetDefaultObject();
	}

	check(self);
	check(self->IsA(func->GetOuterUClass()));
	
	void* params = FMemory::Malloc(func->ParmsSize, func->MinAlignment);
	ON_SCOPE_EXIT { FMemory::Free(params); };

	for (int32 i = 0; i < ParameterProperties.Num(); ++i)
	{
		const TUniquePtr<IZPropertyVisitor>& visitor = ParameterProperties[i];
		visitor->InitializeValue_InContainer(params);
		visitor->SetValue_InContainer(params, buf[staticFunc ? i : i + 1], 0);
	}

	self->ProcessEvent(func, params);

	for (const auto index : OutParamIndices)
	{
		const TUniquePtr<IZPropertyVisitor>& visitor = ParameterProperties[index];
		visitor->GetValue_InContainer(params, buf[staticFunc ? index : index + 1], 0);
	}

	if (ReturnProperty)
	{
		ReturnProperty->GetValue_InContainer(params, buf[-1], 0);
	}
	
	return 0;
}

bool ZSharp::FZCallDispatcher_UFunction::InvalidateCache() const
{
	FString ClassPath;
	FString FunctionName;
	Path.Split(TEXT(":"), &ClassPath, &FunctionName, ESearchCase::CaseSensitive, ESearchDir::FromEnd);
	const UClass* cls = LoadClass<UObject>(nullptr, *ClassPath);
	if (!cls)
	{
		return false;
	}
	
	Function = cls->FindFunctionByName(FName(FunctionName));
	if (!Function.IsValid(true))
	{
		return false;
	}
	
	ParameterProperties.Empty();
	ReturnProperty.Reset();
	OutParamIndices.Empty();
	
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


