// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZManagedDelegateProxyInterface.h"
#include "Interop/ZGCHandle.h"
#include "Reflection/Function/ZFunctionVisitorHandle.h"
#include "Reflection/Wrapper/ZSelfDescriptiveScriptDelegate.h"
#include "ZCall/ZCallHandle.h"

#include "ManagedDelegateProxy.generated.h"

UCLASS()
class ZSHARPRUNTIME_API UManagedDelegateProxy final : public UObject, public IZManagedDelegateProxyInterface
{
	GENERATED_BODY()

	friend ZSharp::FZSelfDescriptiveScriptDelegate;

public:
	virtual ZSharp::FZGCHandle ManagedDelegateProxy_GetDelegate() const override { return Delegate; }

private:
	virtual void BeginDestroy() override;
	virtual void ProcessEvent(UFunction* function, void* parms) override;

private:
	UFUNCTION()
	void __ZStub(){}
	
private:
	TStrongObjectPtr<const UFunction> Signature;
	ZSharp::FZGCHandle Delegate;
	
	ZSharp::FZCallHandle DelegateZCallHandle;
	ZSharp::FZFunctionVisitorHandle SignatureFunctionVisitor;
	
};


