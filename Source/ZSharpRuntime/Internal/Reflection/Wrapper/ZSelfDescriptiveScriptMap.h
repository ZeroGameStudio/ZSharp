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
		ZSHARP_SELF_DESCRIPTIVE_GENERATED_BODY_WITH_OWNS_DESCRIPTOR(FZSelfDescriptiveScriptMap)
		
		FZSelfDescriptiveScriptMap(FZSelfDescriptiveScriptMap&& other) noexcept;
		
		bool TryAdd(const FZCallBufferSlot& key, const FZCallBufferSlot& value);
		bool Remove(const FZCallBufferSlot& key);
		void Clear();
		bool Contains(const FZCallBufferSlot& key) const;
		bool Find(const FZCallBufferSlot& key, FZCallBufferSlot& value) const;
		void Set(const FZCallBufferSlot& key, const FZCallBufferSlot& value);

		int32 Num() const;

		FScriptMapHelper GetHelper() const
		{
			return FScriptMapHelper::CreateHelperFormInnerProperties(const_cast<FProperty*>(Descriptor->Key), const_cast<FProperty*>(Descriptor->Value), UnderlyingInstance);
		}

		void Get(FScriptMapHelper::FIterator it, FZCallBufferSlot& key, FZCallBufferSlot& value) const;

		void AddReferencedObjects(FReferenceCollector& collector);

		FZSelfDescriptiveScriptMap& operator=(FZSelfDescriptiveScriptMap&& other) noexcept;
	
	private:
		void DeleteUnderlyingInstance();
		void DeleteDescriptor();
		void Finalize();

	private:
		TUniquePtr<IZPropertyVisitor> KeyPropertyVisitor;
		TUniquePtr<IZPropertyVisitor> ValuePropertyVisitor;

	};
}


