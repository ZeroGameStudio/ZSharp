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
	
	struct FZPropertyDefinition : public FZFieldDefinition
	{
		// @TODO
	};

	struct FZStructDefinition : public FZFieldDefinition
	{
		FName SuperPath;
		TMap<FName, FZPropertyDefinition> PropertyMap;
	};

	struct FZFunctionDefinition : public FZStructDefinition
	{
		UZSharpFunction* Function = nullptr;
		
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
		UZSharpClass* Class = nullptr;
		
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
	
	struct FZPackageDefinition
	{
		UPackage* Package = nullptr;
		
		FName Path;
		
		TMap<FName, FZEnumDefinition> EnumMap;
		TMap<FName, FZScriptStructDefinition> StructMap;
		TMap<FName, FZClassDefinition> ClassMap;
		TMap<FName, FZInterfaceDefinition> InterfaceMap;
		TMap<FName, FZDelegateDefinition> DelegateMap;
	};
}


