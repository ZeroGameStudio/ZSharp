// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "SoftClassPtr.h"
#include "StrongObjectPtr.h"
#include "Concept/ZStaticExportableClass.h"

namespace ZSharp
{
	constexpr uint16 GUObjectConjugateRegistryId = 1;
	constexpr uint16 GUStructConjugateRegistryId = 2;
	constexpr uint16 GArrayConjugateRegistryId = 31;
	constexpr uint16 GSetConjugateRegistryId = 32;
	constexpr uint16 GMapConjugateRegistryId = 33;
	
	template <CZStaticExportableClass T, typename = void>
	struct TZConjugateRegistryId;

	template <CZStaticExportableClass T>
	constexpr uint16 TZConjugateRegistryId_V = TZConjugateRegistryId<T>::Value;
}

#define ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(ExportedType, RegistryId) template<> struct ZSharp::TZConjugateRegistryId<ExportedType> { static_assert(RegistryId != GUObjectConjugateRegistryId && RegistryId != GUStructConjugateRegistryId); static constexpr uint16 Value = RegistryId; };


