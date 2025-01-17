// Copyright Zero Games. All Rights Reserved.

#include "ZEnhancedInputComponent_Interop.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZConjugateRegistry_UObject.h"
#include "EnhancedInputComponent.h"
#include "Reflection/Delegate/ZManagedDelegateProxy_EnhancedInput.h"

namespace ZEnhancedInputComponent_Interop_Private
{
	static const UFunction* GetSignature()
	{
		static const UFunction* GSignature = FindObjectChecked<UFunction>(nullptr, TEXT("/Script/EnhancedInput.EnhancedInputActionHandlerDynamicSignature__DelegateSignature"));

		return GSignature;
	}
}

ZSharp::FZConjugateHandle ZSharp::FZEnhancedInputComponent_Interop::BindStatelessAction(FZConjugateHandle self, FZConjugateHandle inputAction, int64 triggerEvent, FZGCHandle delegate)
{
	FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
	auto pSelf = registry.ConjugateUnsafeChecked<UEnhancedInputComponent>(self);
	auto pInputAction = registry.ConjugateUnsafeChecked<const UInputAction>(inputAction);
	ETriggerEvent eTriggerEvent = static_cast<ETriggerEvent>(triggerEvent);

	auto proxy = UZManagedDelegateProxyImpl::Create<UZManagedDelegateProxy_EnhancedInput>(ZEnhancedInputComponent_Interop_Private::GetSignature(), delegate, {});
	proxy->Binding = pSelf->BindAction(pInputAction, eTriggerEvent, proxy, UZManagedDelegateProxyImpl::StubFunctionName).Clone();

	return registry.Conjugate(proxy);
}

ZSharp::FZConjugateHandle ZSharp::FZEnhancedInputComponent_Interop::BindStatefulAction(FZConjugateHandle self, FZConjugateHandle inputAction, int64 triggerEvent, FZGCHandle delegate, FZGCHandle state)
{
	FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
	auto pSelf = registry.ConjugateUnsafeChecked<UEnhancedInputComponent>(self);
	auto pInputAction = registry.ConjugateUnsafeChecked<const UInputAction>(inputAction);
	ETriggerEvent eTriggerEvent = static_cast<ETriggerEvent>(triggerEvent);

	auto proxy = UZManagedDelegateProxyImpl::Create<UZManagedDelegateProxy_EnhancedInput>(ZEnhancedInputComponent_Interop_Private::GetSignature(), delegate, state);
	proxy->Binding = pSelf->BindAction(pInputAction, eTriggerEvent, proxy, UZManagedDelegateProxyImpl::StubFunctionName).Clone();

	return registry.Conjugate(proxy);
}

uint8 ZSharp::FZEnhancedInputComponent_Interop::RemoveBinding(FZConjugateHandle self, FZConjugateHandle handle)
{
	FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
	auto pSelf = registry.ConjugateUnsafeChecked<UEnhancedInputComponent>(self);
	auto pHandle = registry.ConjugateUnsafe<const UZManagedDelegateProxy_EnhancedInput>(handle);

	return pSelf->RemoveBinding(*pHandle->Binding);
}


