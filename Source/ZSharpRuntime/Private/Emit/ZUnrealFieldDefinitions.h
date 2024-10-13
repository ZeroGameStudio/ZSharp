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
		String, Name, Text,
		// Object wrappers
		Object, Class, SoftClass, SoftObject, WeakObject, LazyObject, Interface,
		// Containers
		Struct, Array, Set, Map, Optional,
		// Delegates
		Delegate, MulticastInlineDelegate, // MulticastSparseDelegate is not supported.
		// Special types
		FieldPath, // VerseValue is not supported.
	};

	struct FZSimplePropertyDefinition
	{
		FProperty* Property = nullptr;
		
		EZPropertyType Type = EZPropertyType::None;

		EPropertyFlags PropertyFlags = CPF_None;

		FName DescriptorFieldPath;
	};
	
	struct FZPropertyDefinition : public FZFieldDefinition, public FZSimplePropertyDefinition
	{
		FName RepNotifyName;
		
		FZSimplePropertyDefinition InnerProperty;
		FZSimplePropertyDefinition OuterProperty;
	};

	struct FZStructDefinition : public FZFieldDefinition
	{
		FName SuperPath;
		TArray<FZPropertyDefinition> Properties;
	};

	struct FZFunctionDefinition : public FZStructDefinition
	{
		UFunction* Function = nullptr;
		
		EFunctionFlags FunctionFlags = FUNC_None;

		FString ZCallName;

		uint16 RpcId = 0;
		uint16 RpcResponseId = 0;

		FName CustomThunkName;
	};
	
	struct FZEnumDefinition : public FZFieldDefinition
	{
		// @TODO
	};
	
	struct FZScriptStructDefinition : public FZStructDefinition
	{
		// @TODO
	};

	struct FZClassDefinition : public FZStructDefinition
	{
		struct FPropertyDefault
		{
			FString PropertyChain;
			FString Buffer;
		};

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
		
		FName ConfigName;
		FName WithinPath;

		EClassFlags ClassFlags = CLASS_None;
		EClassCastFlags CastFlags = CASTCLASS_None;

		TArray<FName> ImplementedInterfacePaths;
		
		TArray<FZFunctionDefinition> Functions;

		TArray<FPropertyDefault> PropertyDefaults;
		TArray<FDefaultSubobject> DefaultSubobjects;
		TArray<FDefaultSubobjectOverride> DefaultSubobjectOverrides;
		
		TArray<FName> FieldNotifies;
	};
	
	struct FZInterfaceDefinition : public FZStructDefinition
	{
		// @TODO
	};
	
	struct FZDelegateDefinition : public FZStructDefinition
	{
		// @TODO
	};
	
	struct FZUnrealFieldManifest
	{
		UPackage* Package = nullptr;
		
		FName ModuleName;
		
		TArray<FZEnumDefinition> Enums;
		TArray<FZScriptStructDefinition> Structs;
		TArray<FZClassDefinition> Classes;
		TArray<FZInterfaceDefinition> Interfaces;
		TArray<FZDelegateDefinition> Delegates;
	};
}


