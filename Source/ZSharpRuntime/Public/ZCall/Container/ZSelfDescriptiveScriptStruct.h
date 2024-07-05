// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Trait/ZIsUScriptStruct.h"
#include "UObject/StrongObjectPtr.h"

namespace ZSharp
{
	struct ZSHARPRUNTIME_API FZSelfDescriptiveScriptStruct
	{

		FZSelfDescriptiveScriptStruct(const UScriptStruct* descriptor);
		FZSelfDescriptiveScriptStruct(const UScriptStruct* descriptor, void* underlyingInstance, bool owning);
		FZSelfDescriptiveScriptStruct(const FZSelfDescriptiveScriptStruct&) = delete;
		FZSelfDescriptiveScriptStruct(FZSelfDescriptiveScriptStruct&& other) noexcept { *this = MoveTemp(other); }
		~FZSelfDescriptiveScriptStruct();

		const UScriptStruct* GetDescriptor() const { return Descriptor.Get(); }
		void* GetUnderlyingInstance() const { return UnderlyingInstance; }

		template <typename T>
		requires TZIsUScriptStruct_V<T>
		T* GetUnderlyingInstance() const
		{
			check(TBaseStructure<T>::Get() == Descriptor.Get());
			return static_cast<T*>(UnderlyingInstance);
		}

		FZSelfDescriptiveScriptStruct& operator=(const FZSelfDescriptiveScriptStruct&) = delete;
		FZSelfDescriptiveScriptStruct& operator=(FZSelfDescriptiveScriptStruct&& other) noexcept;

	private:
		TStrongObjectPtr<const UScriptStruct> Descriptor;
		void* UnderlyingInstance;
		bool bOwning;
		
	};
}


