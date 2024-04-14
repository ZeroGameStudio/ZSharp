// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	struct ZSHARPCLR_API FZGCHandle
	{

		void Free();

		FString ToString() const
		{
			return FString::Printf(TEXT("GCHandle: [%llu]"), (uint64)Handle);
		}

		bool operator ==(const FZGCHandle other) const
		{
			return Handle == other.Handle;
		}

		operator bool() const
		{
			return !!Handle;
		}

		friend uint32 GetTypeHash(const FZGCHandle& handle)
		{
			return PointerHash(handle.Handle);
		}
		
		void* Handle;
		
	};
}


