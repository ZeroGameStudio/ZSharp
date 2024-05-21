// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Concept/ZStaticExportableClass.h"

namespace ZSharp
{
	constexpr uint16 GUObjectConjugateRegistryId = 1;
	constexpr uint16 GUStructConjugateRegistryId = 2;
	
	template <CZStaticExportableClass T, typename = void>
	struct TZConjugateRegistryId;

	template <CZStaticExportableClass T>
	constexpr uint16 TZConjugateRegistryId_V = TZConjugateRegistryId<T>::Value;
}

#define ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(ExportedType, RegistryId) template<> struct ZSharp::TZConjugateRegistryId<ExportedType> { static_assert(RegistryId != GUObjectConjugateRegistryId && RegistryId != GUStructConjugateRegistryId); static constexpr uint16 Value = RegistryId; };


