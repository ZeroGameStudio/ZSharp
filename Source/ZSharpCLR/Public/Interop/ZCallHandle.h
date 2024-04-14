// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	struct ZSHARPCLR_API FZCallHandle
	{

		friend struct FAllocZCallHandle;

	private:
		static FZCallHandle Alloc();

	public:
		FZCallHandle()
			: Handle(0){}

	public:
		bool IsValid() const { return !!Handle; }

		friend uint32 GetTypeHash(const FZCallHandle& handle)
		{
			return GetTypeHash(handle.Handle);
		}

		bool operator ==(const FZCallHandle other) const
		{
			return Handle == other.Handle;
		}
		
	private:
		uint64 Handle;
		
	};

	struct FAllocZCallHandle
	{
		friend class FZMasterAssemblyLoadContext;
	private:
		static FZCallHandle Alloc() { return FZCallHandle::Alloc(); }
	};
}


