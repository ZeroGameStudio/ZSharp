// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSelfDescriptiveBase.h"
#include "Reflection/Property/IZPropertyVisitor.h"

namespace ZSharp
{
	struct FZCallBufferSlot;

	template <>
	struct TZSelfDescriptiveTraits<struct FZSelfDescriptiveScriptMap> : FZSelfDescriptiveTraitsBase
	{
		static constexpr bool HasCustomDeleteUnderlyingInstance = true;
		static constexpr bool HasCustomDeleteDescriptor = true;
		static constexpr bool HasFinalize = true;
	};
	
	struct ZSHARPRUNTIME_API FZSelfDescriptiveScriptMap : TZSelfDescriptiveBase<FZSelfDescriptiveScriptMap, TPair<const FProperty*, const FProperty*>, FScriptMap>
	{
		ZSHARP_SELF_DESCRIPTIVE_GENERATED_BODY(FZSelfDescriptiveScriptMap)
		
		FZSelfDescriptiveScriptMap(FZSelfDescriptiveScriptMap&& other) noexcept;
		
		void Add(const FZCallBufferSlot& key, const FZCallBufferSlot& value);
		void Remove(const FZCallBufferSlot& key);
		bool Find(const FZCallBufferSlot& key, FZCallBufferSlot& value) const;
		void Clear();

		int32 Num() const;

		FZSelfDescriptiveScriptMap& operator=(FZSelfDescriptiveScriptMap&& other) noexcept;

	private:
		FScriptMapHelper GetHelper() const
		{
			return FScriptMapHelper::CreateHelperFormInnerProperties(const_cast<FProperty*>(Descriptor->Key), const_cast<FProperty*>(Descriptor->Value), UnderlyingInstance);
		}

	private:
		void DeleteUnderlyingInstance();
		void DeleteDescriptor();
		void Finalize();

	private:
		TUniquePtr<IZPropertyVisitor> KeyPropertyVisitor;
		TUniquePtr<IZPropertyVisitor> ValuePropertyVisitor;

	};
}


