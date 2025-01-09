// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Interop/ZGCHandle.h"

#include "ZManagedDelegateProxy.generated.h"

UINTERFACE(meta = (CannotImplementInterfaceInBlueprint, ZSharpNoExport))
class UZManagedDelegateProxy : public UInterface
{
	GENERATED_BODY()
};

class IZManagedDelegateProxy : public IInterface
{
	GENERATED_BODY()

public:
	virtual ZSharp::FZGCHandle ManagedDelegateProxy_GetDelegate() const = 0;
	virtual TOptional<ZSharp::FZGCHandle> ManagedDelegateProxy_GetState() const = 0;
	
};


