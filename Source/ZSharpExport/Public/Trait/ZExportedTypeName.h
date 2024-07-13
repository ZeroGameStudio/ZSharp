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
			return { FString::Printf(TEXT("%s.%s"), *TZManagedTypeInfo<T>::GetAssemblyName(), *TZManagedTypeInfo<T>::GetModuleName()), TZManagedTypeInfo<T>::GetTypeNameText() };
		}
	};
}


