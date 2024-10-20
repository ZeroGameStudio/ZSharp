// Copyright Zero Games. All Rights Reserved.


#include "Reflection/Wrapper/ZSelfDescriptiveMulticastInlineScriptDelegate.h"

#include "Reflection/Function/ZFunctionVisitorRegistry.h"
#include "ZCall/ZManagedDelegateProxy.h"

ZSharp::FZSelfDescriptiveMulticastInlineScriptDelegate::FZSelfDescriptiveMulticastInlineScriptDelegate(FZSelfDescriptiveMulticastInlineScriptDelegate&& other) noexcept
	: Super(MoveTemp(other))
{
	Visitor = other.Visitor;
}

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

	auto proxy = NewObject<UZManagedDelegateProxy>();
	proxy->Signature = TStrongObjectPtr { Descriptor };
	proxy->Delegate = delegate;
	FScriptDelegate unicast;
	unicast.BindUFunction(proxy, GET_FUNCTION_NAME_CHECKED(UZManagedDelegateProxy, __ZStub));
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

ZSharp::EZCallErrorCode ZSharp::FZSelfDescriptiveMulticastInlineScriptDelegate::Broadcast(FZCallBuffer* buffer)
{
	if (!IsBound())
	{
		return EZCallErrorCode::ExternalError;
	}
	
	if (!Visitor)
	{
		if (Visitor = FZFunctionVisitorRegistry::Get().Get(Descriptor); !Visitor)
		{
			return EZCallErrorCode::DispatcherNotFound;
		}
	}
	
	return Visitor->InvokeMulticastInlineScriptDelegate(buffer);
}

bool ZSharp::FZSelfDescriptiveMulticastInlineScriptDelegate::IsBound() const
{
	return UnderlyingInstance->IsBound();
}

ZSharp::FZSelfDescriptiveMulticastInlineScriptDelegate& ZSharp::FZSelfDescriptiveMulticastInlineScriptDelegate::operator=(FZSelfDescriptiveMulticastInlineScriptDelegate&& other) noexcept
{
	Super::operator=(MoveTemp(other));
	
	Visitor = other.Visitor;

	return *this;
}


