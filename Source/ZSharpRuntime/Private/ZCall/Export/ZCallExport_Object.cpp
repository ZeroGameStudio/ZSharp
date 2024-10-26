// Copyright Zero Games. All Rights Reserved.


#include "ZCall/ZStaticallyExportZCall.h"
#include "ZCall/ZStaticallyExportZCallMacros.h"

namespace ZSharp::ExportObject
{
	namespace ZCallExport_Object_Private
	{
		bool IsGarbage(const UObject* obj) { return ::IsValid(obj); }
		void MarkAsGarbage(UObject* obj) { obj->MarkAsGarbage(); }
		bool IsA(const UObject* obj, const UClass* cls) { return obj->IsA(cls); }
		bool Implements(const UObject* obj, const UClass* interface) { return obj->GetClass()->ImplementsInterface(interface); }
		FString GetName(const UObject* obj) { return GetNameSafe(obj); }
		FString GetPathName(const UObject* obj) { return obj->GetPathName(); }
		UObject* NewObject(UClass* cls, UObject* outer, FName name)
		{
			FStaticConstructObjectParameters params { cls };
			params.Outer = outer ? outer : GetTransientPackage();
			params.Name = name;
			return ::StaticConstructObject_Internal(params);
		}
		UObject* LoadObject(UClass* cls, UObject* outer, const FString& name) { return ::StaticLoadObject(cls, outer, *name); }
		UObject* FindObject(UClass* cls, UObject* outer, const FString& name, bool exactClass) { return ::StaticFindObject(cls, outer, *name, exactClass); }
		UObject* LowLevelFindObject(const FString& name) { return ::StaticFindObject(UObject::StaticClass(), nullptr, *name); }
	}

	ZSHARP_STATIC_EXPORT_ZCALL(UObject::GetClass)
	ZSHARP_STATIC_EXPORT_ZCALL(UObject::GetOuter)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_Object_Private::IsGarbage, UObject::IsGarbage)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_Object_Private::MarkAsGarbage, UObject::MarkAsGarbage)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_Object_Private::IsA, UObject::IsA)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_Object_Private::Implements, UObject::Implements)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_Object_Private::GetPathName, UObject::GetPathName)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_Object_Private::GetName, UObject::GetName)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_Object_Private::NewObject, UObjectGlobals::NewObject)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_Object_Private::LoadObject, UObjectGlobals::LoadObject)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_Object_Private::FindObject, UObjectGlobals::FindObject)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_Object_Private::LowLevelFindObject, UObjectGlobals::LowLevelFindObject)
}


