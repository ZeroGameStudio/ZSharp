// Copyright Zero Games. All Rights Reserved.


#include "ZCall/ZStaticallyExportZCall.h"
#include "ZCall/ZStaticallyExportZCallMacros.h"

namespace ZSharp::ExportStruct
{
	namespace ZCallExport_Struct_Private
	{
		bool IsChildOf(const UStruct* strct, const UStruct* other) { return strct->IsChildOf(other); }
	}

	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_Struct_Private::IsChildOf, UStruct::IsChildOf)
}


