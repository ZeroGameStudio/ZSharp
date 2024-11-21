// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	struct FZUnmanagedFunction
	{
		const TCHAR* TypeName;
		const TCHAR* FieldName;
		void* Address;
	};
	
	struct FZUnmanagedFunctions
	{
		int32 Count;
		FZUnmanagedFunction* Functions;
	};
}

#define ZSHARP_STRINGFY_INNER(Text) TEXT(#Text)
#define ZSHARP_STRINGFY(Text) ZSHARP_STRINGFY_INNER(Text)
#define ZSHARP_COMBINE_MANAGED_TYPE_NAME_INNER(Namespace, TypeName) Namespace.TypeName
#define ZSHARP_COMBINE_MANAGED_TYPE_NAME(Namespace, TypeName) ZSHARP_COMBINE_MANAGED_TYPE_NAME_INNER(Namespace, TypeName)
#define ZSHARP_BUILD_UNMANAGED_FUNCTION(ShortTypeName, FieldName) { ZSHARP_STRINGFY(ZSHARP_COMBINE_MANAGED_TYPE_NAME(ZSHARP_UNMANAGED_FUNCTION_ASSEMBLY, ShortTypeName##_Interop)), TEXT(#FieldName), FZ##ShortTypeName##_Interop::FieldName }


