﻿// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSelfDescriptiveBase.h"
#include "Trait/ZIsUScriptStruct.h"
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

		template <typename T>
		requires TZIsUScriptStruct_V<T>
		T* GetTypedUnderlyingInstance() const
		{
			check(Descriptor->IsChildOf(TBaseStructure<T>::Get()));
			return static_cast<T*>(UnderlyingInstance);
		}

		void AddReferencedObjects(FReferenceCollector& collector);

	private:
		static UnderlyingInstanceType* NewUnderlyingInstance(const DescriptorType* descriptor);
		void DeleteUnderlyingInstance();
		
	};
}


