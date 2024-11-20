// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSelfDescriptiveBase.h"
#include "Trait/ZIsUScriptStruct.h"
#include "UObject/StrongObjectPtr.h"

namespace ZSharp
{
	template <>
	struct TZSelfDescriptiveTraits<struct FZSelfDescriptiveScriptStruct> : FZSelfDescriptiveTraitsBase
	{
		static constexpr bool HasCustomNewUnderlyingInstance = true;
		static constexpr bool HasCustomDeleteUnderlyingInstance = true;
	};
	
	struct ZSHARPRUNTIME_API FZSelfDescriptiveScriptStruct : TZSelfDescriptiveBase<FZSelfDescriptiveScriptStruct, UScriptStruct, void>
	{
		ZSHARP_SELF_DESCRIPTIVE_GENERATED_BODY_AUTO_CTOR(FZSelfDescriptiveScriptStruct)

		void* GetUnderlyingInstance() const { return Super::GetUnderlyingInstance(); }

	private:
		static UnderlyingInstanceType* NewUnderlyingInstance(const DescriptorType* descriptor);
		void DeleteUnderlyingInstance();
		
	};
}


