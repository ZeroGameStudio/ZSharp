// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	struct FZConjugateHandle
	{
		void* Handle;
	};
	
	static_assert(TIsPODType<FZConjugateHandle>::Value, "ConjugateHandle must be POD type!");
	static_assert(sizeof(FZConjugateHandle) == sizeof(void*), "ConjugateHandle must only have a void* member!");

	inline bool IsValid(const FZConjugateHandle& handle)
	{
		return !!handle.Handle;
	}

	inline bool operator ==(const FZConjugateHandle& lhs, const FZConjugateHandle& rhs)
	{
		return lhs.Handle == rhs.Handle;
	}

	inline uint32 GetTypeHash(const FZConjugateHandle& handle)
	{
		return PointerHash(handle.Handle);
	}
}


