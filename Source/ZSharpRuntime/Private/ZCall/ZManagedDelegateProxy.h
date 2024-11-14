// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Interop/ZGCHandle.h"

#include "ZManagedDelegateProxy.generated.h"

UINTERFACE(meta = (CannotImplementInterfaceInBlueprint, ZSharpNoExport))
class UZManagedDelegateProxy : public UInterface
{
	GENERATED_BODY()
};

class ZSHARPRUNTIME_API IZManagedDelegateProxy : public IInterface
{
	GENERATED_BODY()

public:
	virtual ZSharp::FZGCHandle ManagedDelegateProxy_GetDelegate() const = 0;
	
};


