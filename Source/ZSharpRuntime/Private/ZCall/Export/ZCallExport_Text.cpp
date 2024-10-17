// Copyright Zero Games. All Rights Reserved.


#include "ZCall/ZStaticallyExportZCall.h"
#include "ZCall/ZStaticallyExportZCallMacros.h"

namespace ZSharp::ExportText
{
	namespace ZCallExport_Text_Private
	{
		const TCHAR* GetData(const FText& text) { return *text.ToString(); }
		void SetData(FText& text, const TCHAR* data) { text = FText::FromString(data); }
	}
	
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_Text_Private::GetData, FText::GetData)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_Text_Private::SetData, FText::SetData)
}


