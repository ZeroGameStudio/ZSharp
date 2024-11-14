// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZManagedDelegateProxy.h"
#include "Interop/ZGCHandle.h"
#include "Reflection/Function/ZFunctionVisitorHandle.h"
#include "Reflection/Wrapper/ZSelfDescriptiveMulticastInlineScriptDelegate.h"
#include "Reflection/Wrapper/ZSelfDescriptiveMulticastSparseScriptDelegate.h"
#include "Reflection/Wrapper/ZSelfDescriptiveScriptDelegate.h"

#include "ZManagedDelegateProxyImpl.generated.h"

UCLASS(Transient, MinimalAPI, meta = (ZSharpNoExport))
class UZManagedDelegateProxyImpl final : public UObject, public IZManagedDelegateProxy
{
	GENERATED_BODY()

	friend ZSharp::FZSelfDescriptiveScriptDelegate;
	friend ZSharp::FZSelfDescriptiveMulticastInlineScriptDelegate;
	friend ZSharp::FZSelfDescriptiveMulticastSparseScriptDelegate;

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
	
	ZSharp::FZFunctionVisitorHandle SignatureFunctionVisitor;
	
};


