// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	static_assert(sizeof(void*) == 8, "Only 64-bit platforms are supported!");
	
	struct FZConjugateHandle
	{
		
		operator bool() const
		{
			return !!Handle;
		}
		
		void* Handle;
		
	};
	
	static_assert(TIsPODType<FZConjugateHandle>::Value, "ConjugateHandle must be POD type!");
	static_assert(sizeof(FZConjugateHandle) == sizeof(void*), "ConjugateHandle must only have a void* member!");

	inline bool operator ==(const FZConjugateHandle& lhs, const FZConjugateHandle& rhs)
	{
		return lhs.Handle == rhs.Handle;
	}
	
	inline uint32 GetTypeHash(const FZConjugateHandle& handle)
	{
		return PointerHash(handle.Handle);
	}
}


