// Copyright Zero Games. All Rights Reserved.

#include "ZUnrealObject_Interop.h"

#include "INotifyFieldValueChanged.h"
#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZConjugateRegistry_UObject.h"
#include "Interop/ZInteropExceptionHelper.h"
#include "Net/NetPushModelHelpers.h"

namespace ZSharp::ZUnrealObject_Interop_Private
{
	static void GetName(FZConjugateHandle self, FString& result)
	{
		const FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
		auto pSelf = registry.ConjugateUnsafeChecked<const UObject>(self);
		result = pSelf->GetName();
	}

	static void GetPathName(FZConjugateHandle self, FString& result)
	{
		const FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
		auto pSelf = registry.ConjugateUnsafeChecked<const UObject>(self);
		result = pSelf->GetPathName();
	}
	
	static void BroadcastFieldValueChanged(FZConjugateHandle self, const TCHAR* fieldName)
	{
		const FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
		auto pSelf = registry.ConjugateUnsafeChecked<UObject>(self);
		
		// Migrate from UFieldNotificationLibrary because it has no exposed API.
		FFieldNotificationId notificationId { fieldName };
		TScriptInterface<INotifyFieldValueChanged> iSelf = pSelf;
		if (iSelf.GetObject() != nullptr && iSelf.GetInterface() != nullptr && notificationId.IsValid())
		{
			const UE::FieldNotification::FFieldId fieldId = iSelf->GetFieldNotificationDescriptor().GetField(pSelf->GetClass(), notificationId.GetFieldName());
			if (fieldId.IsValid())
			{
				iSelf->BroadcastFieldValueChanged(fieldId);
			}
		}
	}
	
	static void MarkPropertyDirty(FZConjugateHandle self, const TCHAR* propertyName)
	{
		const FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
		auto pSelf = registry.ConjugateUnsafeChecked<UObject>(self);
		UNetPushModelHelpers::MarkPropertyDirty(pSelf, propertyName);
	}
}

ZSharp::FZConjugateHandle ZSharp::FZUnrealObject_Interop::GetClass(FZConjugateHandle self)
{
	TRY
	{
		FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
		auto pSelf = registry.ConjugateUnsafeChecked<const UObject>(self);
		return registry.Conjugate(pSelf->GetClass());
	}
	CATCHR({})
}

void ZSharp::FZUnrealObject_Interop::GetName(FZConjugateHandle self, FString& result)
{
	GUARD(ZUnrealObject_Interop_Private::GetName(self, result));
}

void ZSharp::FZUnrealObject_Interop::GetPathName(FZConjugateHandle self, FString& result)
{
	GUARD(ZUnrealObject_Interop_Private::GetPathName(self, result));
}

ZSharp::FZConjugateHandle ZSharp::FZUnrealObject_Interop::GetOuter(FZConjugateHandle self)
{
	TRY
	{
		FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
		auto pSelf = registry.ConjugateUnsafeChecked<const UObject>(self);
		return registry.Conjugate(pSelf->GetOuter());
	}
	CATCHR({})
}

ZSharp::FZConjugateHandle ZSharp::FZUnrealObject_Interop::GetWorld(FZConjugateHandle self)
{
	TRY
	{
#if WITH_ENGINE
		FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
		auto pSelf = registry.ConjugateUnsafeChecked<const UObject>(self);
		return registry.Conjugate(pSelf->GetWorld());
#else
		return {};
#endif
	}
	CATCHR({})
}

uint8 ZSharp::FZUnrealObject_Interop::IsA(FZConjugateHandle self, FZConjugateHandle cls)
{
	TRY
	{
		const FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
		auto pSelf = registry.ConjugateUnsafeChecked<const UObject>(self);
		auto pCls = registry.ConjugateUnsafeChecked<const UClass>(cls);
		return pSelf->IsA(pCls);
	}
	CATCHR(false)
}

uint8 ZSharp::FZUnrealObject_Interop::IsIn(FZConjugateHandle self, FZConjugateHandle outer)
{
	TRY
	{
		const FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
		auto pSelf = registry.ConjugateUnsafeChecked<const UObject>(self);
		auto pOuter = registry.ConjugateUnsafeChecked<const UObject>(outer);
		return pSelf->IsIn(pOuter);
	}
	CATCHR(false)
}

uint8 ZSharp::FZUnrealObject_Interop::Rename(FZConjugateHandle self, const TCHAR* newName, FZConjugateHandle newOuter)
{
	TRY
	{
		const FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
		auto pSelf = registry.ConjugateUnsafeChecked<UObject>(self);
		auto pNewOuter = registry.ConjugateUnsafeChecked<UObject>(newOuter);
		return pSelf->Rename(newName, pNewOuter);
	}
	CATCHR(false)
}

uint8 ZSharp::FZUnrealObject_Interop::IsValid(FZConjugateHandle self)
{
	TRY
	{
		const FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
		auto pSelf = registry.ConjugateUnsafeChecked<UObject>(self);
		return ::IsValid(pSelf);
	}
	CATCHR(false)
}

void ZSharp::FZUnrealObject_Interop::BroadcastFieldValueChanged(FZConjugateHandle self, const TCHAR* fieldName)
{
	GUARD(ZUnrealObject_Interop_Private::BroadcastFieldValueChanged(self, fieldName));
}

void ZSharp::FZUnrealObject_Interop::MarkPropertyDirty(FZConjugateHandle self, const TCHAR* propertyName)
{
	GUARD(ZUnrealObject_Interop_Private::MarkPropertyDirty(self, propertyName));
}


