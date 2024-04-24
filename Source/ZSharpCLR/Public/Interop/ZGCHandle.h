// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	struct FZGCHandle
	{
		void* Handle;
	};
	
	static_assert(TIsPODType<FZGCHandle>::Value, "GCHandle must be POD type!");
	static_assert(sizeof(FZGCHandle) == sizeof(void*), "GCHandle must only have a void* member!");

	void Free(FZGCHandle handle);

	inline bool IsValid(const FZGCHandle& handle)
	{
		return !!handle.Handle;
	}

	inline bool operator ==(const FZGCHandle& lhs, const FZGCHandle& rhs)
	{
		return lhs.Handle == rhs.Handle;
	}

	inline uint32 GetTypeHash(const FZGCHandle& handle)
	{
		return PointerHash(handle.Handle);
	}
}


