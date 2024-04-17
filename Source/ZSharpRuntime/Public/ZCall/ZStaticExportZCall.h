// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	struct FZCallBuffer;
	
	struct ZSHARPRUNTIME_API FZStaticExportZCall
	{
		
	public:
		FZStaticExportZCall(const FString& name, const TFunction<int32(FZCallBuffer*)> function);
		
	};
}


