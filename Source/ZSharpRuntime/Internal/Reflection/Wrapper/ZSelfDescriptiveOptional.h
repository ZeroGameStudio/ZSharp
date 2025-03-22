// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSelfDescriptiveBase.h"
#include "Reflection/Property/IZPropertyVisitor.h"
#include "UObject/PropertyOptional.h"

namespace ZSharp
{
	struct FZCallBufferSlot;

	template <>
	struct TZSelfDescriptiveTraits<struct FZSelfDescriptiveOptional> : FZSelfDescriptiveTraitsBase
	{
		static constexpr bool HasCustomNewUnderlyingInstance = true;
		static constexpr bool HasCustomDeleteUnderlyingInstance = true;
	};
	
	struct ZSHARPRUNTIME_API FZSelfDescriptiveOptional : TZSelfDescriptiveBase<FZSelfDescriptiveOptional, FProperty, void>
	{
		ZSHARP_SELF_DESCRIPTIVE_GENERATED_BODY_WITH_OWNS_DESCRIPTOR(FZSelfDescriptiveOptional)
		
		FZSelfDescriptiveOptional(FZSelfDescriptiveOptional&& other) noexcept;
		~FZSelfDescriptiveOptional();
		
		bool Get(FZCallBufferSlot& dest) const;
		void Set(const FZCallBufferSlot& src);
		void Reset();
		
		bool IsSet() const;

		void AddReferencedObjects(FReferenceCollector& collector);

		FZSelfDescriptiveOptional& operator=(FZSelfDescriptiveOptional&& other) noexcept;

	private:
		/**
		 * Here the helper is not available so we only allocate memory for the underlying instance.
		 * Let our constructor initialize it.
		 */ 
		static UnderlyingInstanceType* NewUnderlyingInstance(const DescriptorType* descriptor) { return nullptr; }
		/**
		 * Helper is already dead here so we need to delete underlying instance in our destructor.
		 * This is only to tell base class not do anything.
		 */
		void DeleteUnderlyingInstance(){}

	private:
		TUniquePtr<FOptionalProperty> Helper; // There is no dedicated helper class such as FScriptArrayHelper for array, so we use a dummy property object as helper.
		TUniquePtr<IZPropertyVisitor> UnderlyingPropertyVisitor;
	};
}


