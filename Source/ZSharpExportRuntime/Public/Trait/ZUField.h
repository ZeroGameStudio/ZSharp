// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZIsUClass.h"
#include "ZIsUScriptStruct.h"
#include "ZIsUEnum.h"

namespace ZSharp
{
	template <typename, typename = void>
	struct TZUField;

	template <typename T>
	struct TZUField<T, std::enable_if_t<TZIsUClass_V<T>>>
	{
		static UField* Get()
		{
			return T::StaticClass();
		}
	};

	template <typename T>
	struct TZUField<T, std::enable_if_t<TZIsUScriptStruct_V<T>>>
	{
		static UField* Get()
		{
			return TBaseStructure<T>::Get();
		}
	};

	template <typename T>
	struct TZUField<T, std::enable_if_t<TZIsUEnum_V<T>>>
	{
		static UField* Get()
		{
			return T::StaticEnum();
		}
	};
}


