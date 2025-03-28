﻿// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSelfDescriptiveBase.h"
#include "Reflection/Property/IZPropertyVisitor.h"

namespace ZSharp
{
	struct FZCallBufferSlot;

	template <>
	struct TZSelfDescriptiveTraits<struct FZSelfDescriptiveScriptArray> : FZSelfDescriptiveTraitsBase
	{
		static constexpr bool HasCustomDeleteUnderlyingInstance = true;
	};
	
	struct ZSHARPRUNTIME_API FZSelfDescriptiveScriptArray : TZSelfDescriptiveBase<FZSelfDescriptiveScriptArray, FProperty, FScriptArray>
	{
		ZSHARP_SELF_DESCRIPTIVE_GENERATED_BODY_WITH_OWNS_DESCRIPTOR(FZSelfDescriptiveScriptArray)
		
		FZSelfDescriptiveScriptArray(FZSelfDescriptiveScriptArray&& other) noexcept;

		void Insert(int32 index, const FZCallBufferSlot& src);
		void InsertAt(int32 index);
		bool Remove(const FZCallBufferSlot& src);
		void RemoveAt(int32 index);
		void Clear();
		int32 IndexOf(const FZCallBufferSlot& src);
		void Get(int32 index, FZCallBufferSlot& dest) const;
		void Set(int32 index, const FZCallBufferSlot& src);

		int32 Num() const;

		FScriptArrayHelper GetHelper() const
		{
			return FScriptArrayHelper::CreateHelperFormInnerProperty(Descriptor, UnderlyingInstance);
		}

		void AddReferencedObjects(FReferenceCollector& collector);

		FZSelfDescriptiveScriptArray& operator=(FZSelfDescriptiveScriptArray&& other) noexcept;

	private:
		void DeleteUnderlyingInstance();

	private:
		TUniquePtr<IZPropertyVisitor> ElementPropertyVisitor;

	};
}


