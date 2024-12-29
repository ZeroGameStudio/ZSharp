// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZManagedDelegateProxyImpl.h"
#include "EnhancedInputComponent.h"
#include "ZManagedDelegateProxy_EnhancedInput.generated.h"

namespace ZSharp
{
	struct FZEnhancedInputComponent_Interop;
}

UCLASS(Transient, MinimalAPI, meta = (ZSharpNoExport))
class UZManagedDelegateProxy_EnhancedInput final : public UZManagedDelegateProxyImpl
{
	GENERATED_BODY()

	friend struct ZSharp::FZEnhancedInputComponent_Interop;

private:
	TUniquePtr<FEnhancedInputActionEventBinding> Binding;
	
};


