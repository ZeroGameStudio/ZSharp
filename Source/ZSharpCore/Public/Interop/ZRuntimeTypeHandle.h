// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	/**
	 * See ZeroGames.ZSharp.Core.InteropRuntimeTypeHandle.
	 */
	struct FZRuntimeTypeHandle
	{
		operator bool() const
		{
			return !!Handle;
		}
		
		void* Handle;
		
	};
	
	static_assert(TIsPODType<FZRuntimeTypeHandle>::Value, "RuntimeTypeHandle must be POD type!");
	static_assert(sizeof(FZRuntimeTypeHandle) == sizeof(void*), "RuntimeTypeHandle must only have a void* member!");

	inline bool operator ==(const FZRuntimeTypeHandle& lhs, const FZRuntimeTypeHandle& rhs)
	{
		return lhs.Handle == rhs.Handle;
	}

	inline uint32 GetTypeHash(const FZRuntimeTypeHandle& handle)
	{
		return PointerHash(handle.Handle);
	}
}


