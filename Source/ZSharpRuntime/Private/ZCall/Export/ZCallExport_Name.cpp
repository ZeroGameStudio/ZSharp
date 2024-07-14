// Copyright Zero Games. All Rights Reserved.


#include "ZCall/ZStaticallyExportZCall.h"
#include "ZCall/ZStaticallyExportZCallMacros.h"

namespace ZSharp
{
	namespace ZCallExport_Name_Private
	{
		const TCHAR* GetData(const FName& name) { return *name.ToString(); }
		void SetData(FName& name, const TCHAR* data) { name = data; }
	}
	
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_Name_Private::GetData, FName::GetData)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_Name_Private::SetData, FName::SetData)
}


