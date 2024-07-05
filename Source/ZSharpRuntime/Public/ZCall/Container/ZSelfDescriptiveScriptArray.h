// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	struct ZSHARPRUNTIME_API FZSelfDescriptiveScriptArray
	{

		FZSelfDescriptiveScriptArray(const FProperty* elementProperty);
		FZSelfDescriptiveScriptArray(const FProperty* elementProperty, FScriptArray* underlyingInstance, bool owning);
		FZSelfDescriptiveScriptArray(const FZSelfDescriptiveScriptArray&) = delete;
		FZSelfDescriptiveScriptArray(FZSelfDescriptiveScriptArray&& other) noexcept { *this = MoveTemp(other); }
		~FZSelfDescriptiveScriptArray();

		const FProperty* GetElementProperty() const { return ElementProperty; }
		FScriptArray* GetUnderlyingInstance() const { return UnderlyingInstance; }

		void InsertAt(int32 index);
		void RemoveAt(int32 index);
		void* Get(int32 index) const;
		void Set(int32 index, const void* item);

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
		FScriptArray* UnderlyingInstance;
		bool bOwning;
		
	};
}


