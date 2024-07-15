// Copyright Zero Games. All Rights Reserved.


#include "ZCall/ZStaticallyExportZCall.h"
#include "ZCall/ZStaticallyExportZCallMacros.h"

namespace ZSharp
{
	namespace ZCallExport_Object_Private
	{
		FString GetName(const UObject* obj) { return GetNameSafe(obj); }
		FString GetPathName(const UObject* obj) { return obj->GetPathName(); }
		UObject* LoadObject(UClass* cls, UObject* outer, const FString& name) { return ::StaticLoadObject(cls, outer, *name); }
		UObject* FindObject(UClass* cls, UObject* outer, const FString& name, bool exactClass) { return ::StaticFindObject(cls, outer, *name, exactClass); }
		UObject* LowLevelFindObject(const FString& name) { return ::StaticFindObject(UObject::StaticClass(), nullptr, *name); }
	}

	ZSHARP_STATIC_EXPORT_ZCALL(UObject::GetClass)
	ZSHARP_STATIC_EXPORT_ZCALL(UObject::GetOuter)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_Object_Private::GetPathName, UObject::GetPathName)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_Object_Private::GetName, UObject::GetName)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_Object_Private::LoadObject, UObjectGlobals::LoadObject)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_Object_Private::FindObject, UObjectGlobals::FindObject)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_Object_Private::LowLevelFindObject, UObjectGlobals::LowLevelFindObject)
}


