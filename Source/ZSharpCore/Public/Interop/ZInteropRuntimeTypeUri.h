// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	struct FZInteropRuntimeTypeUri
	{
		const TCHAR* AssemblyName;
		const TCHAR* TypeName;
		FZInteropRuntimeTypeUri* TypeParameters;
		int32 NumTypeParameters;
	};
	
	static_assert(TIsPODType<FZInteropRuntimeTypeUri>::Value, "RuntimeTypeLocator must be POD type!");
}


