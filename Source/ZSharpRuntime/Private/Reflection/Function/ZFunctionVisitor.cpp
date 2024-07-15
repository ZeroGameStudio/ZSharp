// Copyright Zero Games. All Rights Reserved.


#include "ZFunctionVisitor.h"

#include "ZCall/ZCallBufferSlotEncoder.h"

int32 ZSharp::FZFunctionVisitor::Invoke_Black(FZCallBuffer* buffer) const
{
	check(bAvailable);

	FZCallBuffer& buf = *buffer;
	UFunction* func = Function.Get();
	UObject* self;
	if (!bIsStatic)
	{
		self = TZCallBufferSlotEncoder<UObject*>::Decode(buf[0]);
		if (!self)
		{
			return 1;
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
		visitor->SetValue_InContainer(params, buf[bIsStatic ? i : i + 1], 0);
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

	return 0;
}

int32 ZSharp::FZFunctionVisitor::Invoke_Red(void* param) const
{
	check(bAvailable);

	checkNoEntry();
	return 1;
}

ZSharp::FZFunctionVisitor::FZFunctionVisitor(UFunction* function)
	: bAvailable(false)
	, bIsStatic(false)
{
	Function = function;
	if (!Function.IsValid(true))
	{
		return;
	}

	check(!function->GetSuperFunction());

	bIsStatic = function->HasAllFunctionFlags(FUNC_Static);
	
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


