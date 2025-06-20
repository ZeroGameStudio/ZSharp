﻿// Copyright Zero Games. All Rights Reserved.

#include "Reflection/Wrapper/ZSelfDescriptiveMulticastSparseScriptDelegate.h"

#include "Reflection/ZReflectionHelper.h"
#include "Reflection/Delegate/ZManagedDelegateProxyImpl.h"
#include "Reflection/Function/ZFunctionVisitorRegistry.h"

ZSharp::FZSelfDescriptiveMulticastSparseScriptDelegate::FZSelfDescriptiveMulticastSparseScriptDelegate(FZSelfDescriptiveMulticastSparseScriptDelegate&& other) noexcept
	: Super(MoveTemp(other))
{
	Visitor = MoveTemp(other.Visitor);
}

void ZSharp::FZSelfDescriptiveMulticastSparseScriptDelegate::AddUFunction(UObject* object, FName name)
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

#if DO_CHECK
	if (!ensureAlways(FZReflectionHelper::IsFunctionBindableToDelegate(func, Descriptor)))
	{
		return;
	}
#endif
	
	FScriptDelegate unicast;
	unicast.BindUFunction(object, name);
	Add(unicast);
}

UObject* ZSharp::FZSelfDescriptiveMulticastSparseScriptDelegate::AddManaged(FZGCHandle delegate)
{
	if (!delegate)
	{
		return nullptr;
	}

	auto proxy = UZManagedDelegateProxyImpl::Create(Descriptor, delegate, {});
	FScriptDelegate unicast;
	unicast.BindUFunction(proxy, UZManagedDelegateProxyImpl::StubFunctionName);
	Add(unicast);

	return proxy;
}

UObject* ZSharp::FZSelfDescriptiveMulticastSparseScriptDelegate::AddManaged(FZGCHandle delegate, FZGCHandle state)
{
	if (!delegate)
	{
		return nullptr;
	}

	auto proxy = UZManagedDelegateProxyImpl::Create(Descriptor, delegate, state);
	FScriptDelegate unicast;
	unicast.BindUFunction(proxy, UZManagedDelegateProxyImpl::StubFunctionName);
	Add(unicast);

	return proxy;
}

void ZSharp::FZSelfDescriptiveMulticastSparseScriptDelegate::RemoveUFunction(const UObject* object, FName name)
{
	FScriptDelegate unicast;
	unicast.BindUFunction(const_cast<UObject*>(object), name);
	UnderlyingInstance->__Internal_Remove(GetOwner(), Descriptor->DelegateName, unicast);
}

void ZSharp::FZSelfDescriptiveMulticastSparseScriptDelegate::RemoveAll(const UObject* object)
{
	if (IsBound())
	{
		// FSparseDelegate has only a bool and this won't change in the future,
		// so we just reinterpret it as a bool.
		auto isBound = reinterpret_cast<bool*>(UnderlyingInstance);
		*isBound = FSparseDelegateStorage::RemoveAll(GetOwner(), Descriptor->DelegateName, object);
	}
}

void ZSharp::FZSelfDescriptiveMulticastSparseScriptDelegate::Clear()
{
	UnderlyingInstance->__Internal_Clear(GetOwner(), Descriptor->DelegateName);
}

ZSharp::EZCallErrorCode ZSharp::FZSelfDescriptiveMulticastSparseScriptDelegate::Broadcast(FZCallBuffer* buffer)
{
	if (!Visitor || !*Visitor)
	{
		if (Visitor = MakePimpl<FZFunctionVisitorHandle>(FZFunctionVisitorRegistry::Get().Get(Descriptor)); !Visitor || !*Visitor)
		{
			return EZCallErrorCode::DispatcherNotFound;
		}
	}
	
	return (*Visitor)->InvokeMulticastSparseScriptDelegate(buffer);
}

bool ZSharp::FZSelfDescriptiveMulticastSparseScriptDelegate::IsBound() const
{
	return UnderlyingInstance->IsBound();
}

bool ZSharp::FZSelfDescriptiveMulticastSparseScriptDelegate::IsBoundToObject(const UObject* object) const
{
	FMulticastScriptDelegate* inner = GetDelegatePtr();
	return inner && inner->GetAllObjects().Contains(object);
}

bool ZSharp::FZSelfDescriptiveMulticastSparseScriptDelegate::Contains(const UObject* object, FName name) const
{
	FMulticastScriptDelegate* inner = GetDelegatePtr();
	return inner && inner->Contains(object, name);
}

FMulticastScriptDelegate* ZSharp::FZSelfDescriptiveMulticastSparseScriptDelegate::GetDelegatePtr() const
{
	if (UnderlyingInstance->IsBound())
	{
		return FSparseDelegateStorage::GetMulticastDelegate(GetOwner(), Descriptor->DelegateName);
	}

	return nullptr;
}

ZSharp::FZSelfDescriptiveMulticastSparseScriptDelegate& ZSharp::FZSelfDescriptiveMulticastSparseScriptDelegate::operator=(FZSelfDescriptiveMulticastSparseScriptDelegate&& other) noexcept
{
	Super::operator=(MoveTemp(other));
	
	Visitor = MoveTemp(other.Visitor);

	return *this;
}

ZSharp::FZSelfDescriptiveMulticastSparseScriptDelegate::UnderlyingInstanceType* ZSharp::FZSelfDescriptiveMulticastSparseScriptDelegate::NewUnderlyingInstance(const DescriptorType* descriptor)
{
	checkNoEntry();
	return nullptr;
}

void ZSharp::FZSelfDescriptiveMulticastSparseScriptDelegate::Add(const FScriptDelegate& unicast)
{
	UnderlyingInstance->__Internal_AddUnique(GetOwner(), Descriptor->DelegateName, unicast);
}

const UObject* ZSharp::FZSelfDescriptiveMulticastSparseScriptDelegate::GetOwner() const
{
	return FSparseDelegateStorage::ResolveSparseOwner(*UnderlyingInstance, Descriptor->OwningClassName, Descriptor->DelegateName);
}


