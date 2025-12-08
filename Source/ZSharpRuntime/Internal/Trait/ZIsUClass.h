// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	// A type T where T::StaticClass() returns UClass* and T* is convertible to UObject* is regarded as a UClass.
	template <typename, typename = void>
	struct TZIsUClass
	{
		static constexpr bool Value = false;
	};
	
	template <typename T>
	struct TZIsUClass<T, std::enable_if_t<std::is_same_v<decltype(T::StaticClass()), UClass*> && TPointerIsConvertibleFromTo<std::decay_t<T>, UObject>::Value>>
	{
		static constexpr bool Value = true;
	};
	
	template <typename T>
	constexpr bool TZIsUClass_V = TZIsUClass<T>::Value;
}


