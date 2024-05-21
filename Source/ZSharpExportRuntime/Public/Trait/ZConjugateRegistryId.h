// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "SoftClassPtr.h"
#include "StrongObjectPtr.h"
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

ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(FString, 11)
ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(FName, 12)
ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(FText, 13)

ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(FWeakObjectPtr, 21)
ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(FSoftObjectPtr, 22)
ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(FLazyObjectPtr, 23)
ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(FStrongObjectPtr, 24)
ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(FSoftClassPtr, 25)
ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(FScriptInterface, 26)


