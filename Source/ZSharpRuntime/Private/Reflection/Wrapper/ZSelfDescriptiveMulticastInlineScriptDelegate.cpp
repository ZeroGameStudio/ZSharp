// Copyright Zero Games. All Rights Reserved.


#include "Reflection/Wrapper/ZSelfDescriptiveMulticastInlineScriptDelegate.h"

#include "Reflection/Delegate/ZManagedDelegateProxyImpl.h"
#include "Reflection/Function/ZFunctionVisitorRegistry.h"

ZSharp::FZSelfDescriptiveMulticastInlineScriptDelegate::FZSelfDescriptiveMulticastInlineScriptDelegate(FZSelfDescriptiveMulticastInlineScriptDelegate&& other) noexcept
	: Super(MoveTemp(other))
{
	Visitor = MoveTemp(other.Visitor);
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

	auto proxy = UZManagedDelegateProxyImpl::Create(Descriptor, delegate, {});
	FScriptDelegate unicast;
	unicast.BindUFunction(proxy, UZManagedDelegateProxyImpl::StubFunctionName);
	UnderlyingInstance->Add(unicast);

	return proxy;
}

UObject* ZSharp::FZSelfDescriptiveMulticastInlineScriptDelegate::AddManaged(FZGCHandle delegate, FZGCHandle state)
{
	if (!delegate)
	{
		return nullptr;
	}

	auto proxy = UZManagedDelegateProxyImpl::Create(Descriptor, delegate, state);
	FScriptDelegate unicast;
	unicast.BindUFunction(proxy, UZManagedDelegateProxyImpl::StubFunctionName);
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
	
	if (!Visitor || !*Visitor)
	{
		if (Visitor = MakePimpl<FZFunctionVisitorHandle>(FZFunctionVisitorRegistry::Get().Get(Descriptor)); !Visitor || !*Visitor)
		{
			return EZCallErrorCode::DispatcherNotFound;
		}
	}
	
	return (*Visitor)->InvokeMulticastInlineScriptDelegate(buffer);
}

bool ZSharp::FZSelfDescriptiveMulticastInlineScriptDelegate::IsBound() const
{
	return UnderlyingInstance->IsBound();
}

bool ZSharp::FZSelfDescriptiveMulticastInlineScriptDelegate::IsBoundToObject(const UObject* object) const
{
	return UnderlyingInstance->GetAllObjects().Contains(object);
}

bool ZSharp::FZSelfDescriptiveMulticastInlineScriptDelegate::Contains(const UObject* object, FName name) const
{
	return UnderlyingInstance->Contains(object, name);
}

ZSharp::FZSelfDescriptiveMulticastInlineScriptDelegate& ZSharp::FZSelfDescriptiveMulticastInlineScriptDelegate::operator=(FZSelfDescriptiveMulticastInlineScriptDelegate&& other) noexcept
{
	Super::operator=(MoveTemp(other));
	
	Visitor = MoveTemp(other.Visitor);

	return *this;
}


