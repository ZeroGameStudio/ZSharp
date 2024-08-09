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

ZSharp::EZCallErrorCode ZSharp::FZSelfDescriptiveScriptDelegate::Execute(FZCallBuffer* buffer)
{
	if (!IsBound())
	{
		checkNoEntry();
		return EZCallErrorCode::ExternalError;
	}
	
	if (!Visitor)
	{
		if (Visitor = FZFunctionVisitorRegistry::Get().Get(Descriptor); !Visitor)
		{
			return EZCallErrorCode::DispatcherNotFound;
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
	
	Visitor = other.Visitor;

	return *this;
}


