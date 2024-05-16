// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	namespace ZFullyDecay_Private
	{
		template <typename T>
		struct TZRemoveAllPointer : std::conditional_t<std::is_same_v<T, std::remove_pointer_t<T>>, std::remove_pointer<T>, TZRemoveAllPointer<std::remove_pointer_t<T>>>{};

		template <typename T>
		using TZRemoveAllPointer_T = typename TZRemoveAllPointer<T>::type;
	}
	
	template <typename T>
	concept CZIsFullyDecayed = !std::is_reference_v<T> && !std::is_pointer_v<T> && !std::is_const_v<T> && !std::is_volatile_v<T>;
	
	template <typename T>
	struct TZFullyDecay
	{
		using Type = ZFullyDecay_Private::TZRemoveAllPointer_T<std::decay_t<T>>;
	};

	template <typename T>
	using TZFullyDecay_T = typename TZFullyDecay<T>::Type;
}


