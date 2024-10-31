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
		
		void Add(const FZCallBufferSlot& src);
		void Remove(const FZCallBufferSlot& src);
		bool Contains(const FZCallBufferSlot& src) const;
		void Clear();

		int32 Num() const;

		FZSelfDescriptiveScriptSet& operator=(FZSelfDescriptiveScriptSet&& other) noexcept;

	private:
		FScriptSetHelper GetHelper() const
		{
			return FScriptSetHelper::CreateHelperFormElementProperty(const_cast<FProperty*>(Descriptor), UnderlyingInstance);
		}

	private:
		void DeleteUnderlyingInstance();

	private:
		TUniquePtr<IZPropertyVisitor> ElementPropertyVisitor;

	};
}


