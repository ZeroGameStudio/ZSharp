// Copyright Zero Games. All Rights Reserved.


#include "Reflection/Wrapper/ZSelfDescriptiveMulticastSparseScriptDelegate.h"

#include "Reflection/Function/ZFunctionVisitorRegistry.h"
#include "ZCall/ManagedDelegateProxy.h"

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

	if (!func->IsSignatureCompatibleWith(Descriptor))
	{
		return;
	}
	
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

	auto proxy = NewObject<UManagedDelegateProxy>();
	proxy->Signature = TStrongObjectPtr { Descriptor };
	proxy->Delegate = delegate;
	FScriptDelegate unicast;
	unicast.BindUFunction(proxy, GET_FUNCTION_NAME_CHECKED(UManagedDelegateProxy, __ZStub));
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
		struct { bool bIsBound; }* hack = reinterpret_cast<decltype(hack)>(UnderlyingInstance);
		hack->bIsBound = FSparseDelegateStorage::RemoveAll(GetOwner(), Descriptor->DelegateName, object);
	}
}

void ZSharp::FZSelfDescriptiveMulticastSparseScriptDelegate::Clear()
{
	UnderlyingInstance->__Internal_Clear(GetOwner(), Descriptor->DelegateName);
}

ZSharp::EZCallErrorCode ZSharp::FZSelfDescriptiveMulticastSparseScriptDelegate::Broadcast(FZCallBuffer* buffer)
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
	
	return Visitor->InvokeMulticastSparseScriptDelegate(buffer);
}

bool ZSharp::FZSelfDescriptiveMulticastSparseScriptDelegate::IsBound() const
{
	return UnderlyingInstance->IsBound();
}

FMulticastScriptDelegate* ZSharp::FZSelfDescriptiveMulticastSparseScriptDelegate::GetDelegatePtr()
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
	
	Visitor = other.Visitor;

	return *this;
}

void ZSharp::FZSelfDescriptiveMulticastSparseScriptDelegate::Add(const FScriptDelegate& unicast)
{
	UnderlyingInstance->__Internal_AddUnique(GetOwner(), Descriptor->DelegateName, unicast);
}

const UObject* ZSharp::FZSelfDescriptiveMulticastSparseScriptDelegate::GetOwner() const
{
	return FSparseDelegateStorage::ResolveSparseOwner(*UnderlyingInstance, Descriptor->OwningClassName, Descriptor->DelegateName);
}


