// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	struct FZRuntimeTypeLocator
	{
		const TCHAR* AssemblyName;
		const TCHAR* TypeName;
		FZRuntimeTypeLocator* TypeParameters;
		int32 NumTypeParameters;
	};
	
	static_assert(TIsPODType<FZRuntimeTypeLocator>::Value, "RuntimeTypeLocator must be POD type!");
}


