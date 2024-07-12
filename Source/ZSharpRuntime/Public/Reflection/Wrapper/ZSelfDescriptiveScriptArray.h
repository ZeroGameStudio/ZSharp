// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Reflection/Property/IZPropertyVisitor.h"
#include "Trait/ZConjugateRegistryId.h"

namespace ZSharp
{
	struct FZCallBufferSlot;
	
	struct ZSHARPRUNTIME_API FZSelfDescriptiveScriptArray
	{

		FZSelfDescriptiveScriptArray(const FProperty* elementProperty);
		FZSelfDescriptiveScriptArray(const FProperty* elementProperty, FScriptArray* underlyingInstance);
		FZSelfDescriptiveScriptArray(const FZSelfDescriptiveScriptArray&) = delete;
		FZSelfDescriptiveScriptArray(FZSelfDescriptiveScriptArray&& other) noexcept { *this = MoveTemp(other); }
		~FZSelfDescriptiveScriptArray();

		const FProperty* GetElementProperty() const { return ElementProperty; }
		FScriptArray* GetUnderlyingInstance() const { return UnderlyingInstance; }

		void InsertAt(int32 index);
		void RemoveAt(int32 index);
		void Get(int32 index, FZCallBufferSlot& dest) const;
		void Set(int32 index, const FZCallBufferSlot& src);

		int32 Num() const;

		FZSelfDescriptiveScriptArray& operator=(const FZSelfDescriptiveScriptArray&) = delete;
		FZSelfDescriptiveScriptArray& operator=(FZSelfDescriptiveScriptArray&& other) noexcept;

	private:
		FScriptArrayHelper GetHelper() const
		{
			return FScriptArrayHelper::CreateHelperFormInnerProperty(ElementProperty, UnderlyingInstance);
		}

	private:
		TStrongObjectPtr<UStruct> GCRoot;
		const FProperty* ElementProperty;
		TUniquePtr<IZPropertyVisitor> ElementPropertyVisitor;
		FScriptArray* UnderlyingInstance;
		bool bOwning;
		
	};
}

ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(ZSharp::FZSelfDescriptiveScriptArray, 31)


