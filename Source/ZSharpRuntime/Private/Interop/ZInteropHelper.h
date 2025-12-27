// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	struct FZInteropHelper
	{
		static void CopyTransformUnaligned(const FTransform& src, FTransform& dest)
		{
			static_assert(sizeof(FTransform) == 96);
			static_assert(alignof(FTransform) == 16);
			static_assert(std::is_trivially_copyable_v<FTransform>);
			
			if (reinterpret_cast<uint64>(&src) % alignof(FTransform) == 0 &&
				reinterpret_cast<uint64>(&dest) % alignof(FTransform) == 0)
			{
				dest = src;
			}
			else
			{
				FMemory::Memcpy(&dest, &src, sizeof(FTransform));
			}
		}
	};
}


