// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	struct FZCallHandle
	{
		uint64 Handle;
	};
	
	static_assert(TIsPODType<FZCallHandle>::Value, "ZCallHandle must be POD type!");
	static_assert(sizeof(FZCallHandle) == sizeof(uint64), "ZCallHandle must only have a uint64 member!");

	FZCallHandle AllocateZCallHandle();

	inline bool IsValid(const FZCallHandle& handle)
	{
		return !!handle.Handle;
	}

	inline bool operator ==(const FZCallHandle& lhs, const FZCallHandle& rhs)
	{
		return lhs.Handle == rhs.Handle;
	}

	inline uint32 GetTypeHash(const FZCallHandle& handle)
	{
		return ::GetTypeHash(handle.Handle);
	}
}


