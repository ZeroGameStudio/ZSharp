// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Concept/ZStaticallyExportableClass.h"
#include "Reflection/Wrapper/ZSelfDescriptiveScriptArray.h"
#include "Reflection/Wrapper/ZSelfDescriptiveObjectWrappers.h"

namespace ZSharp
{
	constexpr uint16 GUObjectConjugateRegistryId = 1;
	constexpr uint16 GUStructConjugateRegistryId = 2;
	
	template <CZStaticallyExportableClass T, typename = void>
	struct TZConjugateRegistryId;

	template <CZStaticallyExportableClass T>
	constexpr uint16 TZConjugateRegistryId_V = TZConjugateRegistryId<T>::Value;
}

#define ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(ExportedType, RegistryId) template<> struct ZSharp::TZConjugateRegistryId<ExportedType> { static_assert(RegistryId != GUObjectConjugateRegistryId && RegistryId != GUStructConjugateRegistryId); static constexpr uint16 Value = RegistryId; };

ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(FString, 11)
ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(FName, 12)
ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(FText, 13)
ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(FFieldPath, 14)

ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(ZSharp::FZSelfDescriptiveSubclassOf, 21)
ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(ZSharp::FZSelfDescriptiveSoftClassPtr, 22)
ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(ZSharp::FZSelfDescriptiveSoftObjectPtr, 23)
ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(ZSharp::FZSelfDescriptiveWeakObjectPtr, 24)
ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(ZSharp::FZSelfDescriptiveLazyObjectPtr, 25)
ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(ZSharp::FZSelfDescriptiveScriptInterface, 26)
ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(ZSharp::FZSelfDescriptiveStrongObjectPtr, 27)

ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(ZSharp::FZSelfDescriptiveScriptArray, 31)


