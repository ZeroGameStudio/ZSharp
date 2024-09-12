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

	struct FZSimplePropertyDefinition : public FZFieldDefinition
	{
		FProperty* Property = nullptr;
		
		EZPropertyType Type = EZPropertyType::None;

		EPropertyFlags PropertyFlags = CPF_None;

		FName RepNotifyName;

		FName DescriptorFieldPath; // UField metadata.
		FName MetaDescriptorFieldPath; // UClass metadata.
	};
	
	struct FZPropertyDefinition : public FZSimplePropertyDefinition
	{
		FZSimplePropertyDefinition InnerProperty;
		FZSimplePropertyDefinition OuterProperty;
	};

	struct FZStructDefinition : public FZFieldDefinition
	{
		FName SuperPath;
		TMap<FName, FZPropertyDefinition> PropertyMap;
	};

	struct FZFunctionDefinition : public FZStructDefinition
	{
		UFunction* Function = nullptr;
		
		EFunctionFlags FunctionFlags = FUNC_None;

		uint16 RpcId = 0;
		uint16 RpcResponseId = 0;
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
		UClass* Class = nullptr;
		
		FName ConfigName;
		FName WithinPath;

		EClassFlags ClassFlags = CLASS_None;
		EClassCastFlags CastFlags = CASTCLASS_None;

		TArray<FName> ImplementedInterfacePaths;
		
		TMap<FName, FZFunctionDefinition> FunctionMap;
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
		
		TMap<FName, FZEnumDefinition> EnumMap;
		TMap<FName, FZScriptStructDefinition> StructMap;
		TMap<FName, FZClassDefinition> ClassMap;
		TMap<FName, FZInterfaceDefinition> InterfaceMap;
		TMap<FName, FZDelegateDefinition> DelegateMap;
	};
}


