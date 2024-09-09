// Copyright Zero Games. All Rights Reserved.

#pragma once

class UZSharpClass;
class UZSharpFunction;

// IMPORTANT: These types are designed to use on stack only and use UObject raw pointer, NEVER store them on heap!
namespace ZSharp
{
	struct FZPropertyDefinition
	{
		// @TODO
	};

	struct FZFunctionDefinition
	{
		UZSharpFunction* Function = nullptr;
		
		FString Name;
		
		FString OuterPath;
		FString SuperPath;

		EObjectFlags Flags = RF_NoFlags;
		EFunctionFlags FunctionFlags = FUNC_None;

		uint16 RpcId = 0;
		uint16 RpcResponseId = 0;
		
		TArray<FZPropertyDefinition> Parameters;

		TMap<FName, FString> Metadata;
	};
	
	struct FZEnumDefinition
	{
		// @TODO
	};
	
	struct FZStructDefinition
	{
		// @TODO
	};

	struct FZClassDefinition
	{
		UZSharpClass* Class = nullptr;
		
		FString Name;

		FName ConfigName;
		FString SuperPath;
		FString WithinPath;

		EObjectFlags Flags = RF_NoFlags;
		EClassFlags ClassFlags = CLASS_None;
		EClassCastFlags CastFlags = CASTCLASS_None;

		TArray<FString> ImplementedInterfacePaths;
		
		TArray<FZFunctionDefinition> Functions;
		TArray<FZPropertyDefinition> Properties;

		TMap<FName, FString> Metadata;
	};
	
	struct FZInterfaceDefinition
	{
		// @TODO
	};
	
	struct FZDelegateDefinition
	{
		// @TODO
	};
	
	struct FZPackageDefinition
	{
		UPackage* Package = nullptr;
		
		FString Path;
		
		TArray<FZEnumDefinition> Enums;
		TArray<FZStructDefinition> Structs;
		TArray<FZClassDefinition> Classes;
		TArray<FZInterfaceDefinition> Interfaces;
		TArray<FZDelegateDefinition> Delegates;

		TMap<FName, FString> Metadata;
	};
}


