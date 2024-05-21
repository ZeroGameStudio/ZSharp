// Copyright Zero Games. All Rights Reserved.


#include "Static/ZStaticExportedClass.h"
#include "ZCall/ZStaticExportZCall.h"
#include "ZCall/ZStaticExportZCallMacros.h"

namespace ZSharp
{
	namespace ZCallExport_String_Private
	{
		const TCHAR* GetData(const FString& str) { return *str; }
		void SetData(FString& str, const TCHAR* data) { str = data; }
	}

	ZSHARP_DECLARE_EXPORTED_CLASS(FString, UnrealString, Core)
	ZSHARP_BEGIN_EXPORT_CLASS(FString)
	ZSHARP_END_EXPORT_Class(FString)
	
	ZSHARP_STATIC_EXPORT_ZCALL(FString::Len)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_String_Private::GetData, FString::GetData)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_String_Private::SetData, FString::SetData)
}


