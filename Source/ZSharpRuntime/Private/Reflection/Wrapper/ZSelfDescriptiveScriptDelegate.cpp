// Copyright Zero Games. All Rights Reserved.


#include "Reflection/Wrapper/ZSelfDescriptiveScriptDelegate.h"

#include "Reflection/Function/ZFunctionVisitorRegistry.h"
#include "ZCall/ManagedDelegateProxy.h"

void ZSharp::FZSelfDescriptiveScriptDelegate::BindUFunction(UObject* object, FName name)
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

UObject* ZSharp::FZSelfDescriptiveScriptDelegate::BindManaged(FZGCHandle delegate)
{
	if (!delegate)
	{
		return nullptr;
	}

	auto proxy = NewObject<UManagedDelegateProxy>();
	proxy->Signature = TStrongObjectPtr { Descriptor };
	proxy->Delegate = delegate;
	UnderlyingInstance->BindUFunction(proxy, GET_FUNCTION_NAME_CHECKED(UManagedDelegateProxy, __ZStub));

	return proxy;
}

void ZSharp::FZSelfDescriptiveScriptDelegate::Unbind()
{
	UnderlyingInstance->Unbind();
}

int32 ZSharp::FZSelfDescriptiveScriptDelegate::Execute(FZCallBuffer* buffer)
{
	if (!Visitor)
	{
		if (Visitor = FZFunctionVisitorRegistry::Get().Get(Descriptor); !Visitor)
		{
			return 1;
		}
	}
	
	return Visitor->InvokeScriptDelegate(buffer);
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
