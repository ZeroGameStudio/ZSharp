// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZManagedDelegateProxy.h"
#include "Interop/ZGCHandle.h"
#include "Reflection/Function/ZFunctionVisitorHandle.h"

#include "ZManagedDelegateProxyImpl.generated.h"

UCLASS(Transient, MinimalAPI, meta = (ZSharpNoExport))
class UZManagedDelegateProxyImpl : public UObject, public IZManagedDelegateProxy
{
	GENERATED_BODY()

public:
	static const FName StubFunctionName;
	
	template <typename T = UZManagedDelegateProxyImpl>
	requires TIsDerivedFrom<T, UZManagedDelegateProxyImpl>::Value
	static T* Create(const UFunction* signature, ZSharp::FZGCHandle delegate, TOptional<ZSharp::FZGCHandle> state)
	{
		auto result = NewObject<T>();
		result->Signature = TStrongObjectPtr { signature };
		result->Delegate = delegate;
		result->State = state;
		return result;
	}

public:
	virtual ZSharp::FZGCHandle ManagedDelegateProxy_GetDelegate() const override { return Delegate; }
	virtual TOptional<ZSharp::FZGCHandle> ManagedDelegateProxy_GetState() const override { return State; }

private:
	virtual void BeginDestroy() override;
	virtual void ProcessEvent(UFunction* function, void* parms) override;

private:
	UFUNCTION()
	void __ZStub(){}
	
private:
	TStrongObjectPtr<const UFunction> Signature;
	ZSharp::FZGCHandle Delegate;
	TOptional<ZSharp::FZGCHandle> State;
	
	ZSharp::FZFunctionVisitorHandle SignatureFunctionVisitor;
	
};


