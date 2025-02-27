// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Concept/ZStaticallyExportableClass.h"
#include "Reflection/Wrapper/ZSelfDescriptiveMulticastInlineScriptDelegate.h"
#include "Reflection/Wrapper/ZSelfDescriptiveMulticastSparseScriptDelegate.h"
#include "Reflection/Wrapper/ZSelfDescriptiveScriptArray.h"
#include "Reflection/Wrapper/ZSelfDescriptiveObjectWrappers.h"
#include "Reflection/Wrapper/ZSelfDescriptiveOptional.h"
#include "Reflection/Wrapper/ZSelfDescriptiveScriptDelegate.h"
#include "Reflection/Wrapper/ZSelfDescriptiveScriptMap.h"
#include "Reflection/Wrapper/ZSelfDescriptiveScriptSet.h"
#include "Reflection/Wrapper/ZSelfDescriptiveScriptStruct.h"

namespace ZSharp
{
	constexpr uint16 GUObjectConjugateRegistryId = 1;

	template <CZStaticallyExportableClass T, typename = void>
	struct TZConjugateRegistryId;

	template <CZStaticallyExportableClass T>
	constexpr uint16 TZConjugateRegistryId_V = TZConjugateRegistryId<T>::Value;
}

#define ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(ExportedType, RegistryId) template<> struct ZSharp::TZConjugateRegistryId<ExportedType> { static_assert(RegistryId != GUObjectConjugateRegistryId); static constexpr uint16 Value = RegistryId; };

ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(ZSharp::FZSelfDescriptiveScriptStruct, 2)

ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(FString, 11)
ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(FUtf8String, 12)
ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(FAnsiString, 13)
ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(FName, 14)
ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(FText, 15)

ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(ZSharp::FZSelfDescriptiveSubclassOf, 21)
ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(ZSharp::FZSelfDescriptiveSoftClassPtr, 22)
ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(ZSharp::FZSelfDescriptiveSoftObjectPtr, 23)
ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(ZSharp::FZSelfDescriptiveWeakObjectPtr, 24)
ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(ZSharp::FZSelfDescriptiveLazyObjectPtr, 25)
ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(ZSharp::FZSelfDescriptiveScriptInterface, 26)

ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(ZSharp::FZSelfDescriptiveScriptArray, 31)
ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(ZSharp::FZSelfDescriptiveScriptSet, 32)
ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(ZSharp::FZSelfDescriptiveScriptMap, 33)
ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(ZSharp::FZSelfDescriptiveOptional, 34)

ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(ZSharp::FZSelfDescriptiveScriptDelegate, 41)
ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(ZSharp::FZSelfDescriptiveMulticastInlineScriptDelegate, 42)
ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(ZSharp::FZSelfDescriptiveMulticastSparseScriptDelegate, 43)

ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(FFieldPath, 51)


