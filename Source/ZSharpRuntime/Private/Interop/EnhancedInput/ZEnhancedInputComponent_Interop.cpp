// Copyright Zero Games. All Rights Reserved.

#include "ZEnhancedInputComponent_Interop.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZConjugateRegistry_UObject.h"
#include "EnhancedInputComponent.h"
#include "Reflection/Delegate/ZManagedDelegateProxy_EnhancedInput.h"

namespace ZSharp::ZEnhancedInputComponent_Interop_Private
{
	FZConjugateHandle BindAction(FZConjugateHandle self, FZConjugateHandle inputAction, int64 triggerEvent, FZGCHandle delegate, TOptional<FZGCHandle> state, TFunctionRef<void(UZManagedDelegateProxy_EnhancedInput&, uint32)> setupBinding)
	{
		static const UFunction* GSignature = FindObjectChecked<UFunction>(nullptr, TEXT("/Script/EnhancedInput.EnhancedInputActionHandlerDynamicSignature__DelegateSignature"));
		
		FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
		auto pSelf = registry.ConjugateUnsafeChecked<UEnhancedInputComponent>(self);
		auto pInputAction = registry.ConjugateUnsafeChecked<const UInputAction>(inputAction);
		ETriggerEvent eTriggerEvent = static_cast<ETriggerEvent>(triggerEvent);
	
		auto proxy = UZManagedDelegateProxyImpl::Create<UZManagedDelegateProxy_EnhancedInput>(GSignature, delegate, state);
		setupBinding(*proxy, pSelf->BindAction(pInputAction, eTriggerEvent, proxy, UZManagedDelegateProxyImpl::StubFunctionName).GetHandle());
	
		return registry.Conjugate(proxy);
	}
}

uint32 ZSharp::FZEnhancedInputComponent_Interop::BindUFunctionAction(FZConjugateHandle self, FZConjugateHandle inputAction, int64 triggerEvent, FZConjugateHandle obj, const TCHAR* functionName)
{
	FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
	auto pSelf = registry.ConjugateUnsafeChecked<UEnhancedInputComponent>(self);
	auto pInputAction = registry.ConjugateUnsafeChecked<const UInputAction>(inputAction);
	ETriggerEvent eTriggerEvent = static_cast<ETriggerEvent>(triggerEvent);
	auto pObj = registry.ConjugateUnsafeChecked<UObject>(obj);

	return pSelf->BindAction(pInputAction, eTriggerEvent, pObj, functionName).GetHandle();
}

ZSharp::FZConjugateHandle ZSharp::FZEnhancedInputComponent_Interop::BindStatelessAction(FZConjugateHandle self, FZConjugateHandle inputAction, int64 triggerEvent, FZGCHandle delegate)
{
	return ZEnhancedInputComponent_Interop_Private::BindAction(self, inputAction, triggerEvent, delegate, {}, [](UZManagedDelegateProxy_EnhancedInput& proxy, uint32 handle){ proxy.Binding = handle; });
}

ZSharp::FZConjugateHandle ZSharp::FZEnhancedInputComponent_Interop::BindStatefulAction(FZConjugateHandle self, FZConjugateHandle inputAction, int64 triggerEvent, FZGCHandle delegate, FZGCHandle state)
{
	return ZEnhancedInputComponent_Interop_Private::BindAction(self, inputAction, triggerEvent, delegate, state, [](UZManagedDelegateProxy_EnhancedInput& proxy, uint32 handle){ proxy.Binding = handle; });
}

uint8 ZSharp::FZEnhancedInputComponent_Interop::RemoveBindingByHandle(FZConjugateHandle self, uint32 handle)
{
	FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
	auto pSelf = registry.ConjugateUnsafeChecked<UEnhancedInputComponent>(self);

	return pSelf->RemoveBindingByHandle(handle);
}

uint8 ZSharp::FZEnhancedInputComponent_Interop::RemoveBinding(FZConjugateHandle self, FZConjugateHandle handle)
{
	FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
	auto pSelf = registry.ConjugateUnsafeChecked<UEnhancedInputComponent>(self);
	auto pHandle = registry.ConjugateUnsafe<const UZManagedDelegateProxy_EnhancedInput>(handle);
	if (!ensure(pHandle))
	{
		return false;
	}

	return pSelf->RemoveBindingByHandle(pHandle->Binding);
}


