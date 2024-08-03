// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZFullyExportedTypeName.h"
#include "Trait/ZManagedTypeInfo.h"

namespace ZSharp
{
	template <typename T>
	struct TZExportedTypeName
	{
		static FZFullyExportedTypeName Get()
		{
			return { TZManagedTypeInfo<T>::GetNamespace(), TZManagedTypeInfo<T>::GetTypeNameText() };
		}
	};
}


