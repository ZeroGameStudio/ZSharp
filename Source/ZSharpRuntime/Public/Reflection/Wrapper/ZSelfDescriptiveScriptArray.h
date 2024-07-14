// Copyright Zero Games. All Rights Reserved.

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

		using Super = TZSelfDescriptiveBase;
		friend struct TZSelfDescriptiveBase;

		FZSelfDescriptiveScriptArray(const DescriptorType* descriptor);
		FZSelfDescriptiveScriptArray(const DescriptorType* descriptor, UnderlyingInstanceType* underlyingInstance);
		FZSelfDescriptiveScriptArray(FZSelfDescriptiveScriptArray&& other) noexcept;

		void InsertAt(int32 index);
		void RemoveAt(int32 index);
		void Get(int32 index, FZCallBufferSlot& dest) const;
		void Set(int32 index, const FZCallBufferSlot& src);

		int32 Num() const;

		FZSelfDescriptiveScriptArray& operator=(FZSelfDescriptiveScriptArray&& other) noexcept;

	private:
		FScriptArrayHelper GetHelper() const
		{
			return FScriptArrayHelper::CreateHelperFormInnerProperty(Descriptor, UnderlyingInstance);
		}

	private:
		void DeleteUnderlyingInstance();

	private:
		TUniquePtr<IZPropertyVisitor> ElementPropertyVisitor;

	};
}


