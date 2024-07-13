// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	struct FZPropertyDesc
	{
		void* Descriptor;
		void* Metadata;
	};

	static_assert(TIsPODType<FZPropertyDesc>::Value);
	
	struct FZPropertyFactory
	{
		static FProperty* Create(const FZPropertyDesc& desc);
	};
}


