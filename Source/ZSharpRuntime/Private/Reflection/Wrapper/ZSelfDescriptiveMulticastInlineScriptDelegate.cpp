﻿// Copyright Zero Games. All Rights Reserved.


#include "Reflection/Wrapper/ZSelfDescriptiveMulticastInlineScriptDelegate.h"

#include "Reflection/Function/ZFunctionVisitorRegistry.h"
#include "ZCall/ManagedDelegateProxy.h"

void ZSharp::FZSelfDescriptiveMulticastInlineScriptDelegate::AddUFunction(UObject* object, FName name)
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
	
	FScriptDelegate unicast;
	unicast.BindUFunction(object, name);
	UnderlyingInstance->Add(unicast);
}

UObject* ZSharp::FZSelfDescriptiveMulticastInlineScriptDelegate::AddManaged(FZGCHandle delegate)
{
	if (!delegate)
	{
		return nullptr;
	}

	auto proxy = NewObject<UManagedDelegateProxy>();
	proxy->Signature = TStrongObjectPtr { Descriptor };
	proxy->Delegate = delegate;
	FScriptDelegate unicast;
	unicast.BindUFunction(proxy, GET_FUNCTION_NAME_CHECKED(UManagedDelegateProxy, __ZStub));
	UnderlyingInstance->Add(unicast);

	return proxy;
}

void ZSharp::FZSelfDescriptiveMulticastInlineScriptDelegate::RemoveUFunction(const UObject* object, FName name)
{
	UnderlyingInstance->Remove(object, name);
}

void ZSharp::FZSelfDescriptiveMulticastInlineScriptDelegate::RemoveAll(const UObject* object)
{
	UnderlyingInstance->RemoveAll(object);
}

void ZSharp::FZSelfDescriptiveMulticastInlineScriptDelegate::Clear()
{
	UnderlyingInstance->Clear();
}

int32 ZSharp::FZSelfDescriptiveMulticastInlineScriptDelegate::Broadcast(FZCallBuffer* buffer)
{
	if (!IsBound())
	{
		return 1;
	}
	
	if (!Visitor)
	{
		if (Visitor = FZFunctionVisitorRegistry::Get().Get(Descriptor); !Visitor)
		{
			return 2;
		}
	}
	
	return Visitor->InvokeMulticastInlineScriptDelegate(buffer);
}

bool ZSharp::FZSelfDescriptiveMulticastInlineScriptDelegate::IsBound() const
{
	return UnderlyingInstance->IsBound();
}

