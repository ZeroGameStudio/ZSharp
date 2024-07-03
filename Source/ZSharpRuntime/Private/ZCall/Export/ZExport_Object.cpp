// Copyright Zero Games. All Rights Reserved.


#include "ZCall/ZStaticExportZCall.h"
#include "ZCall/ZStaticExportZCallMacros.h"

namespace ZSharp
{
	namespace ZCallExport_Object_Private
	{
		FString GetName(const UObject* obj) { return GetNameSafe(obj); }
		UObject* LowLevelFindObject(const FString& name) { return ::StaticFindObject(UObject::StaticClass(), nullptr, *name); }
		UObject* FindObject(UClass* cls, UObject* outer, const FString& name, bool exactClass) { return ::StaticFindObject(cls, outer, *name, exactClass); }
	}

	ZSHARP_STATIC_EXPORT_ZCALL(UObject::GetClass)
	ZSHARP_STATIC_EXPORT_ZCALL(UObject::GetOuter)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_Object_Private::GetName, UObject::GetName)

	static TZStaticExportZCall<decltype(&ZCallExport_Object_Private::LowLevelFindObject), &ZCallExport_Object_Private::LowLevelFindObject> LowLevelFindObject { "ex://UObjectGlobals.LowLevelFindObject" };
	static TZStaticExportZCall<decltype(&ZCallExport_Object_Private::FindObject), &ZCallExport_Object_Private::FindObject> FindObject { "ex://UObjectGlobals.FindObject" };
}


