// Copyright Zero Games. All Rights Reserved.

#pragma once
#include "Trait/ZIsUScriptStruct.h"

namespace ZSharp
{
	struct FZSelfDescriptiveScriptStruct final : FNoncopyable
	{

		FZSelfDescriptiveScriptStruct(const UScriptStruct* descriptor);
		FZSelfDescriptiveScriptStruct(const UScriptStruct* descriptor, void* underlyingInstance, bool owning);
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

		FZSelfDescriptiveScriptStruct& operator=(FZSelfDescriptiveScriptStruct&& other) noexcept;

	private:
		TStrongObjectPtr<const UScriptStruct> Descriptor;
		void* UnderlyingInstance;
		bool bOwning;
		
	};
}


