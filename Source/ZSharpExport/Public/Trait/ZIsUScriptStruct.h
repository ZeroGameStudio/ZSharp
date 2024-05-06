// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	// A type T where TBaseStructure<T>::Get() returns UScriptStruct* is regarded as a UScriptStruct.
	template <typename, typename = void>
	struct TZIsUScriptStruct
	{
		static constexpr bool Value = false;
	};
	
	template <typename T>
	struct TZIsUScriptStruct<T, std::enable_if_t<std::is_same_v<decltype(TBaseStructure<T>::Get()), UScriptStruct*>>>
	{
		static constexpr bool Value = true;
	};
	
	template <typename T>
	constexpr bool TZIsUScriptStruct_V = TZIsUScriptStruct<T>::Value;
}


