// Copyright Zero Games. All Rights Reserved.


#include "Reflection/Wrapper/ZSelfDescriptiveScriptDelegate.h"

void ZSharp::FZSelfDescriptiveScriptDelegate::Bind(UObject* object, FName name)
{
	if (!object)
	{
		return;
	}
	
	const UFunction* func = object->FindFunction(name);
	if (!func)
	{
		return;
	}

	if (!func->IsSignatureCompatibleWith(Descriptor))
	{
		return;
	}
	
	UnderlyingInstance->BindUFunction(object, name);
}

void ZSharp::FZSelfDescriptiveScriptDelegate::Unbind()
{
	UnderlyingInstance->Unbind();
}

void ZSharp::FZSelfDescriptiveScriptDelegate::Execute(void* params)
{
	UnderlyingInstance->ProcessDelegate<UObject>(params);
}

UObject* ZSharp::FZSelfDescriptiveScriptDelegate::GetObject() const
{
	return UnderlyingInstance->GetUObject();
}

FName ZSharp::FZSelfDescriptiveScriptDelegate::GetFunctionName() const
{
	return UnderlyingInstance->GetFunctionName();
}

const UFunction* ZSharp::FZSelfDescriptiveScriptDelegate::GetFunction() const
{
	const UObject* object = GetObject();
	if (!object)
	{
		return nullptr;
	}
	
	return object->FindFunctionChecked(GetFunctionName());
}

FString ZSharp::FZSelfDescriptiveScriptDelegate::GetZCallName() const
{
	const UFunction* func = GetFunction();
	if (!func)
	{
		return {};
	}
	
	while (const UFunction* super = func->GetSuperFunction())
	{
		func = super;
	}

	return FString::Printf(TEXT("uf:/%s"), *func->GetPathName());
}

bool ZSharp::FZSelfDescriptiveScriptDelegate::IsBound() const
{
	return UnderlyingInstance->IsBound();
}

bool ZSharp::FZSelfDescriptiveScriptDelegate::IsBoundToObject(const UObject* object) const
{
	return UnderlyingInstance->IsBoundToObject(object);
}
