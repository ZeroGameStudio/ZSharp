// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSelfDescriptiveBase.h"
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

		using Super::GetUnderlyingInstance;

		void AddReferencedObjects(FReferenceCollector& collector);

	private:
		static UnderlyingInstanceType* NewUnderlyingInstance(const DescriptorType* descriptor);
		void DeleteUnderlyingInstance();
		
	};
}


