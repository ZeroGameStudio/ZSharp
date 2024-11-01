// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	/**
	 * See ZeroGames.ZSharp.Core.InteropRuntimeTypeUri.
	 */
	struct FZInteropRuntimeTypeUri
	{
		const TCHAR* AssemblyName;
		const TCHAR* TypeName;
		FZInteropRuntimeTypeUri* TypeParameters;
		int32 NumTypeParameters;
	};
	
	static_assert(TIsPODType<FZInteropRuntimeTypeUri>::Value, "InteropRuntimeTypeUri must be POD type!");
}


