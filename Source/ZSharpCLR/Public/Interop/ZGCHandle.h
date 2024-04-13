// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	struct ZSHARPCLR_API FZGCHandle
	{

		void Free();

		operator bool() const
		{
			return !!Handle;
		}

		FString ToString() const
		{
			return FString::Printf(TEXT("GCHandle: [%llu]"), (uint64)Handle);
		}
		
		void* Handle;
		
	};
}


