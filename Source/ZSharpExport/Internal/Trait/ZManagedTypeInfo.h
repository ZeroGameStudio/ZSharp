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

ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(FString, CoreUObject, UnrealString)
ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(FUtf8String, CoreUObject, UnrealUtf8String)
ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(FAnsiString, CoreUObject, UnrealAnsiString)
ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(FName, CoreUObject, UnrealName)
ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(FText, CoreUObject, UnrealText)

ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(ZSharp::FZSelfDescriptiveScriptArray, CoreUObject, UnrealArray`1)
ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(ZSharp::FZSelfDescriptiveScriptSet, CoreUObject, UnrealSet`1)
ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(ZSharp::FZSelfDescriptiveScriptMap, CoreUObject, UnrealMap`2)
ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(ZSharp::FZSelfDescriptiveOptional, CoreUObject, UnrealOptional`1)

ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(ZSharp::FZSelfDescriptiveSubclassOf, CoreUObject, SubclassOf`1)
ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(ZSharp::FZSelfDescriptiveSoftClassPtr, CoreUObject, SoftClassPtr`1)
ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(ZSharp::FZSelfDescriptiveSoftObjectPtr, CoreUObject, SoftObjectPtr`1)
ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(ZSharp::FZSelfDescriptiveWeakObjectPtr, CoreUObject, WeakObjectPtr`1)
ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(ZSharp::FZSelfDescriptiveLazyObjectPtr, CoreUObject, LazyObjectPtr`1)
ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(ZSharp::FZSelfDescriptiveScriptInterface, CoreUObject, ScriptInterface`1)
ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(ZSharp::FZSelfDescriptiveStrongObjectPtr, CoreUObject, StrongObjectPtr`1)

ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(FFieldPath, CoreUObject, FieldPath)


