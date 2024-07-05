// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	struct FZSimplePropertyDesc
	{
		void* Descriptor;
		void* Metadata;
	};
	
	struct FZPropertyDesc
	{
		void* Descriptor;
		FZSimplePropertyDesc InnerProperty;
		FZSimplePropertyDesc OuterProperty;
	};

	static_assert(TIsPODType<FZSimplePropertyDesc>::Value);
	static_assert(TIsPODType<FZPropertyDesc>::Value);
	
	struct FZPropertyFactory
	{
		static FProperty* Create(const FZPropertyDesc& desc);
	};
}


