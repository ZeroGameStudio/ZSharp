// Copyright Zero Games. All Rights Reserved.


#include "Reflection/Wrapper/ZSelfDescriptiveMulticastScriptDelegate.h"

#include "Reflection/Function/ZFunctionVisitorRegistry.h"
#include "ZCall/ManagedDelegateProxy.h"

void ZSharp::FZSelfDescriptiveMulticastScriptDelegate::AddUFunction(UObject* object, FName name)
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

UObject* ZSharp::FZSelfDescriptiveMulticastScriptDelegate::AddManaged(FZGCHandle delegate)
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

void ZSharp::FZSelfDescriptiveMulticastScriptDelegate::RemoveUFunction(const UObject* object, FName name)
{
	UnderlyingInstance->Remove(object, name);
}

void ZSharp::FZSelfDescriptiveMulticastScriptDelegate::RemoveAll(const UObject* object)
{
	UnderlyingInstance->RemoveAll(object);
}

void ZSharp::FZSelfDescriptiveMulticastScriptDelegate::Clear()
{
	UnderlyingInstance->Clear();
}

int32 ZSharp::FZSelfDescriptiveMulticastScriptDelegate::Broadcast(FZCallBuffer* buffer)
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
	
	return Visitor->InvokeMulticastScriptDelegate(buffer);
}

bool ZSharp::FZSelfDescriptiveMulticastScriptDelegate::IsBound() const
{
	return UnderlyingInstance->IsBound();
}


