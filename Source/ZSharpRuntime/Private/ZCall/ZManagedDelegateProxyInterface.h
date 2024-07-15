// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Interop/ZGCHandle.h"

#include "ZManagedDelegateProxyInterface.generated.h"

UINTERFACE()
class UZManagedDelegateProxyInterface : public UInterface
{
	GENERATED_BODY()
};

class ZSHARPRUNTIME_API IZManagedDelegateProxyInterface : public IInterface
{
	GENERATED_BODY()

public:
	virtual ZSharp::FZGCHandle ManagedDelegateProxy_GetDelegate() const = 0;
	
};
