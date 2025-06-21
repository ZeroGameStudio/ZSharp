// Copyright Zero Games. All Rights Reserved.

#include "Reflection/Wrapper/ZSelfDescriptiveScriptDelegate.h"

#include "Reflection/Delegate/ZManagedDelegateProxyImpl.h"
#include "Reflection/Function/ZFunctionVisitorRegistry.h"

ZSharp::FZSelfDescriptiveScriptDelegate::FZSelfDescriptiveScriptDelegate(FZSelfDescriptiveScriptDelegate&& other) noexcept
	: Super(MoveTemp(other))
{
	Visitor = MoveTemp(other.Visitor);
}

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

	if (!ensureAlways(func->IsSignatureCompatibleWith(Descriptor)))
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

	auto proxy = UZManagedDelegateProxyImpl::Create(Descriptor, delegate, {});
	UnderlyingInstance->BindUFunction(proxy, UZManagedDelegateProxyImpl::StubFunctionName);

	return proxy;
}

UObject* ZSharp::FZSelfDescriptiveScriptDelegate::BindManaged(FZGCHandle delegate, FZGCHandle state)
{
	if (!delegate)
	{
		return nullptr;
	}

	auto proxy = UZManagedDelegateProxyImpl::Create(Descriptor, delegate, state);
	UnderlyingInstance->BindUFunction(proxy, UZManagedDelegateProxyImpl::StubFunctionName);

	return proxy;
}

void ZSharp::FZSelfDescriptiveScriptDelegate::Unbind()
{
	UnderlyingInstance->Unbind();
}

ZSharp::EZCallErrorCode ZSharp::FZSelfDescriptiveScriptDelegate::Execute(FZCallBuffer* buffer)
{
	if (!IsBound())
	{
		checkNoEntry();
		return EZCallErrorCode::ExternalError;
	}
	
	if (!Visitor || !*Visitor)
	{
		if (Visitor = MakePimpl<FZFunctionVisitorHandle>(FZFunctionVisitorRegistry::Get().Get(Descriptor)); !Visitor || !*Visitor)
		{
			return EZCallErrorCode::DispatcherNotFound;
		}
	}
	
	return (*Visitor)->InvokeScriptDelegate(buffer);
}

UObject* ZSharp::FZSelfDescriptiveScriptDelegate::GetObject() const
{
	return UnderlyingInstance->GetUObject();
}

FName ZSharp::FZSelfDescriptiveScriptDelegate::GetFunctionName() const
{
	return UnderlyingInstance->GetFunctionName();
}

bool ZSharp::FZSelfDescriptiveScriptDelegate::IsBound() const
{
	return UnderlyingInstance->IsBound();
}

bool ZSharp::FZSelfDescriptiveScriptDelegate::IsBoundToObject(const UObject* object) const
{
	return UnderlyingInstance->IsBoundToObject(object);
}

ZSharp::FZSelfDescriptiveScriptDelegate& ZSharp::FZSelfDescriptiveScriptDelegate::operator=(FZSelfDescriptiveScriptDelegate&& other) noexcept
{
	Super::operator=(MoveTemp(other));
	
	Visitor = MoveTemp(other.Visitor);

	return *this;
}


