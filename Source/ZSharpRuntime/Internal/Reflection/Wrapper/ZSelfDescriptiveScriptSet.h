// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSelfDescriptiveBase.h"
#include "Reflection/Property/IZPropertyVisitor.h"

namespace ZSharp
{
	struct FZCallBufferSlot;

	template <>
	struct TZSelfDescriptiveTraits<struct FZSelfDescriptiveScriptSet> : FZSelfDescriptiveTraitsBase
	{
		static constexpr bool HasCustomDeleteUnderlyingInstance = true;
	};
	
	struct ZSHARPRUNTIME_API FZSelfDescriptiveScriptSet : TZSelfDescriptiveBase<FZSelfDescriptiveScriptSet, FProperty, FScriptSet>
	{
		ZSHARP_SELF_DESCRIPTIVE_GENERATED_BODY_WITH_OWNS_DESCRIPTOR(FZSelfDescriptiveScriptSet)
		
		FZSelfDescriptiveScriptSet(FZSelfDescriptiveScriptSet&& other) noexcept;
		
		bool Add(const FZCallBufferSlot& src);
		bool Remove(const FZCallBufferSlot& src);
		void Clear();
		bool Contains(const FZCallBufferSlot& src) const;

		int32 Num() const;

		FScriptSetHelper GetHelper() const
		{
			return FScriptSetHelper::CreateHelperFormElementProperty(const_cast<FProperty*>(Descriptor), UnderlyingInstance);
		}

		void Get(FScriptSetHelper::FIterator it, FZCallBufferSlot& dest);

		FZSelfDescriptiveScriptSet& operator=(FZSelfDescriptiveScriptSet&& other) noexcept;

	private:
		void DeleteUnderlyingInstance();

	private:
		TUniquePtr<IZPropertyVisitor> ElementPropertyVisitor;

	};
}


