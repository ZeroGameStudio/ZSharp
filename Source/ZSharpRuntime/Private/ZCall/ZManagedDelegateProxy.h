﻿// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZManagedDelegateProxyInterface.h"
#include "Interop/ZGCHandle.h"
#include "Reflection/Function/ZFunctionVisitorHandle.h"
#include "Reflection/Wrapper/ZSelfDescriptiveMulticastInlineScriptDelegate.h"
#include "Reflection/Wrapper/ZSelfDescriptiveMulticastSparseScriptDelegate.h"
#include "Reflection/Wrapper/ZSelfDescriptiveScriptDelegate.h"

#include "ZManagedDelegateProxy.generated.h"

UCLASS(MinimalAPI)
class UZManagedDelegateProxy final : public UObject, public IZManagedDelegateProxyInterface
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


