// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	struct FZRuntimeTypeUri
	{
		FString AssemblyName;
		FString TypeName;
		TArray<FZRuntimeTypeUri> TypeParameters;
	};
}


