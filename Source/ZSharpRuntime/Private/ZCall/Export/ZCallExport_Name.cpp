// Copyright Zero Games. All Rights Reserved.


#include "ZCall/ZStaticallyExportZCall.h"
#include "ZCall/ZStaticallyExportZCallMacros.h"

namespace ZSharp
{
	namespace ZCallExport_Name_Private
	{
		FString ToString(const FName& name) { return name.ToString(); }
		void SetData(FName& name, const TCHAR* data) { name = data; }
	}
	
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_Name_Private::ToString, FName::ToString)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_Name_Private::SetData, FName::SetData)
}


