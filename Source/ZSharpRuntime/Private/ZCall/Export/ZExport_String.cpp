// Copyright Zero Games. All Rights Reserved.


#include "Static/ZStaticallyExportedClass.h"
#include "ZCall/ZStaticallyExportZCall.h"
#include "ZCall/ZStaticallyExportZCallMacros.h"

namespace ZSharp
{
	namespace ZCallExport_String_Private
	{
		const TCHAR* GetData(const FString& str) { return *str; }
		void SetData(FString& str, const TCHAR* data) { str = data; }
	}
	
	ZSHARP_STATIC_EXPORT_ZCALL(FString::Len)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_String_Private::GetData, FString::GetData)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_String_Private::SetData, FString::SetData)
}

ZSHARP_BEGIN_EXPORT_CLASS(FString)
ZSHARP_END_EXPORT_CLASS(FString)


