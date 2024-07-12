// Copyright Zero Games. All Rights Reserved.

#pragma once
#include "Reflection/Wrapper/ZSelfDescriptiveObjectWrappers.h"
#include "Reflection/Wrapper/ZSelfDescriptiveScriptArray.h"

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
		static FString GetModuleName() { return "System"; } \
		static FString GetTypeName() { return #Name; } \
		static FString GetFullName() { return FString::Printf(TEXT("System.%s"), TEXT(#Name)); } \
	};

#define ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(Type, Module, Name) \
	template<> \
	struct ZSharp::TZManagedTypeInfo<Type> \
	{ \
		static FString GetAssemblyName() { return ZSHARP_ENGINE_ASSEMBLY_NAME; } \
		static FString GetModuleName() { return #Module; } \
		static FString GetTypeName() { return #Name; } \
		static FString GetPureTypeName() { FString res; GetTypeName().Split("`", &res, nullptr); return res; } \
		static FString GetFullName() { return FString::Printf(TEXT("%s.%s.%s"), TEXT(ZSHARP_ENGINE_ASSEMBLY_NAME), TEXT(#Module), TEXT(#Name)); } \
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

ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(FString, Core, UnrealString)
ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(FName, Core, UnrealName)
ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(FText, Core, UnrealText)

ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(ZSharp::FZSelfDescriptiveScriptArray, Core, UnrealArray`1)
// ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(ZSharp::FZSelfDescriptiveScriptSet, Core, UnrealSet`1)
// ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(ZSharp::FZSelfDescriptiveScriptMap, Core, UnrealMap`2)
// ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(ZSharp::FZSelfDescriptiveScriptOptional, Core, UnrealOptional`1)

// ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(ZSharp::FSelfDescriptiveScriptDelegate, Core, UnrealScriptDelegate`1)
// ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(ZSharp::FSelfDescriptiveMulticastScriptDelegate, Core, UnrealMulticastScriptDelegate`1)

ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(ZSharp::FZSelfDescriptiveSubclassOf, System, SubclassOf`1)
ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(ZSharp::FZSelfDescriptiveSoftClassPtr, System, SoftClassPtr`1)
ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(ZSharp::FZSelfDescriptiveSoftObjectPtr, System, SoftObjectPtr`1)
ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(ZSharp::FZSelfDescriptiveWeakObjectPtr, System, WeakObjectPtr`1)
ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(ZSharp::FZSelfDescriptiveLazyObjectPtr, System, LazyObjectPtr`1)
ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(ZSharp::FZSelfDescriptiveScriptInterface, System, ScriptInterface`1)

ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(FFieldPath, CoreUObject, FieldPath)


