// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	struct FZCallHandle
	{

		static FZCallHandle Alloc();

		bool IsRed() const { return Handle < 0; }
		bool IsBlack() const { return Handle > 0; }

		operator bool() const
		{
			return !!Handle;
		}
		
		int64 Handle;
		
	};
	
	static_assert(TIsPODType<FZCallHandle>::Value, "ZCallHandle must be POD type!");
	static_assert(sizeof(FZCallHandle) == sizeof(int64), "ZCallHandle must only have a int64 member!");

	inline bool operator ==(const FZCallHandle& lhs, const FZCallHandle& rhs)
	{
		return lhs.Handle == rhs.Handle;
	}

	inline uint32 GetTypeHash(const FZCallHandle& handle)
	{
		return ::GetTypeHash(handle.Handle);
	}
}


