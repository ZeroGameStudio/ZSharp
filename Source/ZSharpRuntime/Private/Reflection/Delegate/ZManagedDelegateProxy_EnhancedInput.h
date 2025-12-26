// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZManagedDelegateProxyImpl.h"
#include "ZManagedDelegateProxy_EnhancedInput.generated.h"

namespace ZSharp
{
	struct FZEnhancedInputComponent_Interop;
	namespace ZEnhancedInputComponent_Interop_Private
	{
		struct FZBinder;
	}
}

UCLASS(Transient, MinimalAPI, meta = (ZSharpNoExport))
class UZManagedDelegateProxy_EnhancedInput final : public UZManagedDelegateProxyImpl
{
	GENERATED_BODY()

	friend struct ZSharp::FZEnhancedInputComponent_Interop;
	friend struct ZSharp::ZEnhancedInputComponent_Interop_Private::FZBinder;
	
private:
	uint32 Binding;
	
};


