// Copyright Zero Games. All Rights Reserved.


#include "ZCall/ZStaticExportZCall.h"
#include "ZCall/ZStaticExportZCallMacros.h"

namespace ZSharp
{
	namespace ZCallExport_Object_Private
	{
		FString GetName(const UObject* obj) { return GetNameSafe(obj); }
	}

	ZSHARP_STATIC_EXPORT_ZCALL(UObject::GetClass)
	ZSHARP_STATIC_EXPORT_ZCALL(UObject::GetOuter)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_Object_Private::GetName, UObject::GetName)
}


