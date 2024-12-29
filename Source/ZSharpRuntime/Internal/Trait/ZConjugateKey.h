// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Reflection/Wrapper/ZSelfDescriptiveScriptArray.h"
#include "Reflection/Wrapper/ZSelfDescriptiveScriptSet.h"
#include "Reflection/Wrapper/ZSelfDescriptiveScriptMap.h"
#include "Reflection/Wrapper/ZSelfDescriptiveOptional.h"

#include "Reflection/Wrapper/ZSelfDescriptiveObjectWrappers.h"

namespace ZSharp
{
	// These two dummy structs are designed to represent blueprint structs and events on managed side.
	struct FZDynamicScriptStruct;
	struct FZDynamicMulticastInlineDelegate;
	
	template <typename>
	struct TZConjugateKey;
}

#define CONJUGATE_KEY(Type, Key) \
template<> \
struct ZSharp::TZConjugateKey<Type> { inline static FString Value = #Key; };

CONJUGATE_KEY(uint8, @System.Byte)
CONJUGATE_KEY(uint16, @System.UInt16)
CONJUGATE_KEY(uint32, @System.UInt32)
CONJUGATE_KEY(uint64, @System.UInt64)
CONJUGATE_KEY(int8, @System.SByte)
CONJUGATE_KEY(int16, @System.Int16)
CONJUGATE_KEY(int32, @System.Int32)
CONJUGATE_KEY(int64, @System.Int64)
CONJUGATE_KEY(float, @System.Single)
CONJUGATE_KEY(double, @System.Double)
CONJUGATE_KEY(bool, @System.Boolean)

CONJUGATE_KEY(FString, Unreal.String)
CONJUGATE_KEY(FUtf8String, Unreal.Utf8String)
CONJUGATE_KEY(FAnsiString, Unreal.AnsiString)
CONJUGATE_KEY(FName, Unreal.Name)
CONJUGATE_KEY(FText, Unreal.Text)

CONJUGATE_KEY(ZSharp::FZSelfDescriptiveScriptArray, Unreal.Array)
CONJUGATE_KEY(ZSharp::FZSelfDescriptiveScriptSet, Unreal.Set)
CONJUGATE_KEY(ZSharp::FZSelfDescriptiveScriptMap, Unreal.Map)
CONJUGATE_KEY(ZSharp::FZSelfDescriptiveOptional, Unreal.Optional)

CONJUGATE_KEY(ZSharp::FZSelfDescriptiveSubclassOf, Unreal.SubclassOf)
CONJUGATE_KEY(ZSharp::FZSelfDescriptiveSoftClassPtr, Unreal.SoftClassPtr)
CONJUGATE_KEY(ZSharp::FZSelfDescriptiveSoftObjectPtr, Unreal.SoftObjectPtr)
CONJUGATE_KEY(ZSharp::FZSelfDescriptiveWeakObjectPtr, Unreal.WeakObjectPtr)
CONJUGATE_KEY(ZSharp::FZSelfDescriptiveLazyObjectPtr, Unreal.LazyObjectPtr)
CONJUGATE_KEY(ZSharp::FZSelfDescriptiveScriptInterface, Unreal.ScriptInterface)
CONJUGATE_KEY(ZSharp::FZSelfDescriptiveStrongObjectPtr, Unreal.StrongObjectPtr)

CONJUGATE_KEY(FFieldPath, Unreal.FieldPath)

CONJUGATE_KEY(ZSharp::FZDynamicScriptStruct, Unreal.DynamicScriptStruct)
CONJUGATE_KEY(ZSharp::FZDynamicMulticastInlineDelegate, Unreal.DynamicMulticastInlineDelegate)

#undef CONJUGATE_KEY


