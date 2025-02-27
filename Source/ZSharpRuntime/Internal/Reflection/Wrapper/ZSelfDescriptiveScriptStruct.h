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
	
	struct ZSHARPRUNTIME_API FZSelfDescriptiveScriptStruct : TZSelfDescriptiveBase<FZSelfDescriptiveScriptStruct, UScriptStruct, void>, public FGCObject
	{
		ZSHARP_SELF_DESCRIPTIVE_GENERATED_BODY_AUTO_CTOR(FZSelfDescriptiveScriptStruct)

		using Super::GetUnderlyingInstance;

	private:
		virtual void AddReferencedObjects(FReferenceCollector& collector) override
		{
			if (bOwning)
			{
				collector.AddPropertyReferencesWithStructARO(Descriptor, UnderlyingInstance);
			}
		}
		
		virtual FString GetReferencerName() const override
		{
			return "SelfDescriptiveScriptStruct";
		}

	private:
		static UnderlyingInstanceType* NewUnderlyingInstance(const DescriptorType* descriptor);
		void DeleteUnderlyingInstance();
		
	};
}


