// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Interop/ZGCHandle.h"
#include "Reflection/Wrapper/ZSelfDescriptiveScriptDelegate.h"

#include "ManagedDelegateProxy.generated.h"

UCLASS()
class ZSHARPRUNTIME_API UManagedDelegateProxy final : public UObject
{
	GENERATED_BODY()

	friend ZSharp::FZSelfDescriptiveScriptDelegate;

private:
	virtual void BeginDestroy() override;
	virtual void ProcessEvent(UFunction* function, void* parms) override;

private:
	UFUNCTION()
	void __ZStub(){}
	
private:
	TStrongObjectPtr<const UFunction> Signature;
	ZSharp::FZGCHandle Delegate;
	
};


