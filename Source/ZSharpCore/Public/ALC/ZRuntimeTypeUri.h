// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	/**
	 * See ZeroGames.ZSharp.Core.RuntimeTypeUri
	 */
	struct FZRuntimeTypeUri
	{
		FString AssemblyName;
		FString TypeName;
		TArray<FZRuntimeTypeUri> TypeParameters;
	};
}


