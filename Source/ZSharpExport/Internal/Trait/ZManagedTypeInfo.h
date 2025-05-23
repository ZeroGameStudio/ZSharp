// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Reflection/Wrapper/ZSelfDescriptiveScriptArray.h"
#include "Reflection/Wrapper/ZSelfDescriptiveScriptSet.h"
#include "Reflection/Wrapper/ZSelfDescriptiveScriptMap.h"
#include "Reflection/Wrapper/ZSelfDescriptiveOptional.h"

#include "Reflection/Wrapper/ZSelfDescriptiveObjectWrappers.h"

namespace ZSharp
{
	template <typename>
	struct TZManagedTypeInfo;
}

#define ZSHARP_DECLARE_SYSTEM_MANAGED_TYPE_INFO(Type, Name) \
	template<> \
	struct ZSharp::TZManagedTypeInfo<Type> \
	{ \
		static FString GetAssemblyName() { return "System"; } \
		static FString GetModuleName() { return GetAssemblyName(); } \
		static FString GetNamespace() { return GetAssemblyName(); } \
		static FString GetTypeName() { return #Name; } \
		static FString GetTypeNameText() { return #Type; } \
		static FString GetFullName() { return FString::Printf(TEXT("%s.%s"), *GetNamespace(), *GetTypeName()); } \
	};

#define ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(Type, Module, Name) \
	template<> \
	struct ZSharp::TZManagedTypeInfo<Type> \
	{ \
		static FString GetAssemblyName() { return ZSHARP_ENGINE_ASSEMBLY_NAME; } \
		static FString GetModuleName() { return #Module; } \
		static FString GetNamespace() { return FString::Printf(TEXT("%s.%s"), *GetAssemblyName(), *GetModuleName()); } \
		static FString GetTypeName() { return #Name; } \
		static FString GetTypeNameText() { FString res; return GetTypeName().Split("`", &res, nullptr) ? res : GetTypeName(); } \
		static FString GetFullName() { return FString::Printf(TEXT("%s.%s"), *GetNamespace(), *GetTypeName()); } \
	};

ZSHARP_DECLARE_SYSTEM_MANAGED_TYPE_INFO(uint8, Byte)
ZSHARP_DECLARE_SYSTEM_MANAGED_TYPE_INFO(uint16, UInt16)
ZSHARP_DECLARE_SYSTEM_MANAGED_TYPE_INFO(uint32, UInt32)
ZSHARP_DECLARE_SYSTEM_MANAGED_TYPE_INFO(uint64, UInt64)
ZSHARP_DECLARE_SYSTEM_MANAGED_TYPE_INFO(int8, SByte)
ZSHARP_DECLARE_SYSTEM_MANAGED_TYPE_INFO(int16, Int16)
ZSHARP_DECLARE_SYSTEM_MANAGED_TYPE_INFO(int32, Int32)
ZSHARP_DECLARE_SYSTEM_MANAGED_TYPE_INFO(int64, Int64)
ZSHARP_DECLARE_SYSTEM_MANAGED_TYPE_INFO(float, Single)
ZSHARP_DECLARE_SYSTEM_MANAGED_TYPE_INFO(double, Double)
ZSHARP_DECLARE_SYSTEM_MANAGED_TYPE_INFO(bool, Boolean)

ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(FString, CoreUObject, FString)
ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(FUtf8String, CoreUObject, FUtf8String)
ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(FAnsiString, CoreUObject, FAnsiString)
ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(FName, CoreUObject, FName)
ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(FText, CoreUObject, FText)

ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(ZSharp::FZSelfDescriptiveScriptArray, CoreUObject, TArray`1)
ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(ZSharp::FZSelfDescriptiveScriptSet, CoreUObject, TSet`1)
ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(ZSharp::FZSelfDescriptiveScriptMap, CoreUObject, TMap`2)
ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(ZSharp::FZSelfDescriptiveOptional, CoreUObject, TOptional`1)

ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(ZSharp::FZSelfDescriptiveSubclassOf, CoreUObject, TSubclassOf`1)
ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(ZSharp::FZSelfDescriptiveSoftClassPtr, CoreUObject, TSoftClassPtr`1)
ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(ZSharp::FZSelfDescriptiveSoftObjectPtr, CoreUObject, TSoftObjectPtr`1)
ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(ZSharp::FZSelfDescriptiveWeakObjectPtr, CoreUObject, TWeakObjectPtr`1)
ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(ZSharp::FZSelfDescriptiveLazyObjectPtr, CoreUObject, TLazyObjectPtr`1)
ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(ZSharp::FZSelfDescriptiveScriptInterface, CoreUObject, TScriptInterface`1)

ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(FFieldPath, CoreUObject, FFieldPath)


