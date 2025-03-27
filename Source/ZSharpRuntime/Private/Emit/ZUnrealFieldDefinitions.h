// Copyright Zero Games. All Rights Reserved.

#pragma once

class UZSharpClass;
class UZSharpFunction;

// IMPORTANT: These types are designed to use on stack only and use UObject raw pointer, NEVER store them on heap!
namespace ZSharp
{
	struct FZFieldDefinition
	{
		FName Name;
		EObjectFlags Flags = RF_NoFlags;
		TMap<FName, FString> MetadataMap;
		TMap<FName, FString> TransparentDataMap;
	};

	enum class EZPropertyType : uint8
	{
		None,
		// Primitives
		UInt8, UInt16, UInt32, UInt64, Int8, Int16, Int32, Int64, Float, Double, Bool, Enum,
		// Strings
		String, Utf8String, AnsiString, Name, Text,
		// Object wrappers
		Object, Class, SoftClass, SoftObject, WeakObject, LazyObject, Interface,
		// Containers
		Struct, Array, Set, Map, Optional,
		// Delegates
		Delegate, MulticastInlineDelegate, MulticastSparseDelegate,
		// Special types
		FieldPath, // VerseValue is not supported.
	};

	enum class EZEnumUnderlyingType : uint8
	{
		None,
		UInt8,
		UInt16,
		UInt32,
		UInt64,
		Int8,
		Int16,
		Int32,
		Int64,
	};

	enum class EZDelegateType : uint8
	{
		None,
		Unicast,
		Multicast,
		Sparse,
	};

	struct FZSimplePropertyDefinition
	{
		FProperty* Property = nullptr;
		
		EZPropertyType Type = EZPropertyType::None;
		EZEnumUnderlyingType EnumUnderlyingType = EZEnumUnderlyingType::None;
		EPropertyFlags PropertyFlags = CPF_None;

		FName DescriptorFieldPath;
	};
	
	struct FZPropertyDefinition : public FZFieldDefinition, public FZSimplePropertyDefinition
	{
		FName RepNotifyName;
		ELifetimeCondition RepCondition = COND_None;
		ELifetimeRepNotifyCondition RepNotifyCondition = REPNOTIFY_OnChanged;
		bool IsRepPushBased = false;
		
		FZSimplePropertyDefinition InnerProperty;
		FZSimplePropertyDefinition OuterProperty;
	};

	struct FZStructDefinition : public FZFieldDefinition
	{
		struct FPropertyDefault
		{
			FString PropertyChain;
			FString Buffer;
		};
		
		FName SuperPath;
		TArray<FZPropertyDefinition> Properties;
		TArray<FPropertyDefault> PropertyDefaults;
	};

	struct FZFunctionDefinition : public FZStructDefinition
	{
		UFunction* Function = nullptr;

		bool bIsEventOverride = false;
		EFunctionFlags FunctionFlags = FUNC_None;

		FString ZCallName;
		FString ValidateZCallName;

		uint16 RpcId = 0;
		uint16 RpcResponseId = 0;

		FName CustomThunkName;
	};

	struct FZEnumFieldDefinition : public FZFieldDefinition
	{
		int64 Value = 0;
	};
	
	struct FZEnumDefinition : public FZFieldDefinition
	{
		UEnum* Enum = nullptr;

		EZEnumUnderlyingType UnderlyingType = EZEnumUnderlyingType::None;
		EEnumFlags EnumFlags = EEnumFlags::None;

		TArray<FZEnumFieldDefinition> Fields;
	};
	
	struct FZScriptStructDefinition : public FZStructDefinition
	{
		UScriptStruct* ScriptStruct = nullptr;
		
		EStructFlags StructFlags = STRUCT_NoFlags;
	};

	struct FZDelegateDefinition : public FZStructDefinition
	{
		UDelegateFunction* Delegate = nullptr;
		
		EZDelegateType DelegateType = EZDelegateType::None;
		FName OuterClassName;
		EFunctionFlags FunctionFlags = FUNC_None;
	};

	struct FZInterfaceDefinition : public FZStructDefinition
	{
		// @TODO
	};

	struct FZClassDefinition : public FZStructDefinition
	{
		struct FDefaultSubobject
		{
			FName Name;
			FName ClassPath;
			bool bOptional = false;
			bool bTransient = false;
			
			FName PropertyName;
			
			bool bRootComponent = false;
			FName AttachParentDefaultSubobjectName;
			FName AttachSocketName;
		};

		struct FDefaultSubobjectOverride
		{
			FName Name;
			FName ClassPath;
		};
		
		UClass* Class = nullptr;

		bool bConstructor = false;
		bool bContextual = false;
		
		FName ConfigName;
		FName WithinPath;

		EClassFlags ClassFlags = CLASS_None;
		EClassCastFlags CastFlags = CASTCLASS_None;

		TArray<FName> ImplementedInterfacePaths;
		
		TArray<FZFunctionDefinition> Functions;
		
		TArray<FDefaultSubobject> DefaultSubobjects;
		TArray<FDefaultSubobjectOverride> DefaultSubobjectOverrides;
		
		TArray<FName> FieldNotifies;
	};
	
	struct FZUnrealFieldManifest
	{
		UPackage* Package = nullptr;
		
		FName ModuleName;
		
		TArray<FZEnumDefinition> Enums;
		TArray<FZScriptStructDefinition> Structs;
		TArray<FZDelegateDefinition> Delegates;
		TArray<FZInterfaceDefinition> Interfaces;
		TArray<FZClassDefinition> Classes;
	};
}


