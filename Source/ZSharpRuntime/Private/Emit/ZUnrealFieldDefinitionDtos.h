// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZUnrealFieldDefinitionDtos.generated.h"

static_assert(std::is_same_v<__underlying_type(EObjectFlags), int32>);
static_assert(std::is_same_v<__underlying_type(EPropertyFlags), uint64>);
static_assert(std::is_same_v<__underlying_type(EFunctionFlags), uint32>);
static_assert(std::is_same_v<__underlying_type(EClassFlags), int32>);
static_assert(std::is_same_v<__underlying_type(EClassCastFlags), uint64>);
static_assert(std::is_same_v<__underlying_type(EStructFlags), int32>);

USTRUCT()
struct FZFieldDefinitionDto
{
	GENERATED_BODY()

	UPROPERTY()
	FName Name;

	UPROPERTY()
	int32 Flags = 0;
	
	UPROPERTY()
	TMap<FName, FString> MetadataMap;
	
	UPROPERTY()
	TMap<FName, FString> TransparentDataMap;
};

USTRUCT()
struct FZSimplePropertyDefinitionDto
{
	GENERATED_BODY()

	UPROPERTY()
	uint8 Type = 0;

	UPROPERTY()
	uint64 PropertyFlags = 0;

	UPROPERTY()
	FName DescriptorFieldPath;
};

USTRUCT()
struct FZPropertyDefinitionDto : public FZFieldDefinitionDto
{
	GENERATED_BODY()

	// USTRUCT forbids multiple inheritance so we copy fields from FZSimplePropertyDefinitionDto.
	UPROPERTY()
	uint8 Type = 0;

	UPROPERTY()
	uint64 PropertyFlags = 0;

	UPROPERTY()
	FName DescriptorFieldPath;

	UPROPERTY()
	FName RepNotifyName;
	
	UPROPERTY()
	FZSimplePropertyDefinitionDto InnerProperty;

	UPROPERTY()
	FZSimplePropertyDefinitionDto OuterProperty;
	
};

USTRUCT()
struct FZStructDefinitionDto : public FZFieldDefinitionDto
{
	GENERATED_BODY()

	UPROPERTY()
	FName SuperPath;

	UPROPERTY()
	TArray<FZPropertyDefinitionDto> Properties;
};

USTRUCT()
struct FZFunctionDefinitionDto : public FZStructDefinitionDto
{
	GENERATED_BODY()

	UPROPERTY()
	uint32 FunctionFlags = 0;

	UPROPERTY()
	FString ZCallName;

	UPROPERTY()
	uint16 RpcId = 0;

	UPROPERTY()
	uint16 RpcResponseId = 0;
};

USTRUCT()
struct FZEnumDefinitionDto : public FZFieldDefinitionDto
{
	GENERATED_BODY()
	
	// @TODO
};

USTRUCT()
struct FZScriptStructDefinitionDto : public FZStructDefinitionDto
{
	GENERATED_BODY()
	
	// @TODO
};

USTRUCT()
struct FZClassDefinitionDto_PropertyDefault
{
	GENERATED_BODY()

	UPROPERTY()
	FString PropertyChain;

	UPROPERTY()
	FString Buffer;
};

USTRUCT()
struct FZClassDefinitionDto_DefaultSubobject
{
	GENERATED_BODY()

	UPROPERTY()
	FName Name;

	UPROPERTY()
	FName ClassPath;

	UPROPERTY()
	bool bOptional = false;

	UPROPERTY()
	bool bTransient = false;

	UPROPERTY()
	FName PropertyName;

	UPROPERTY()
	bool bRootComponent = false;

	UPROPERTY()
	FName AttachParentDefaultSubobjectName;

	UPROPERTY()
	FName AttachSocketName;
};

USTRUCT()
struct FZClassDefinitionDto_DefaultSubobjectOverride
{
	GENERATED_BODY()

	UPROPERTY()
	FName Name;

	UPROPERTY()
	FName ClassPath;
};

USTRUCT()
struct FZClassDefinitionDto : public FZStructDefinitionDto
{
	GENERATED_BODY()

	UPROPERTY()
	FName ConfigName;

	UPROPERTY()
	FName WithinPath;

	UPROPERTY()
	int32 ClassFlags = 0;

	UPROPERTY()
	uint64 CastFlags = 0;

	UPROPERTY()
	TArray<FName> ImplementedInterfacePaths;

	UPROPERTY()
	TArray<FZFunctionDefinitionDto> Functions;

	UPROPERTY()
	TArray<FZClassDefinitionDto_PropertyDefault> PropertyDefaults;

	UPROPERTY()
	TArray<FZClassDefinitionDto_DefaultSubobject> DefaultSubobjects;

	UPROPERTY()
	TArray<FZClassDefinitionDto_DefaultSubobjectOverride> DefaultSubobjectOverrides;
};

USTRUCT()
struct FZInterfaceDefinitionDto : public FZStructDefinitionDto
{
	GENERATED_BODY()
	
	// @TODO
};

USTRUCT()
struct FZDelegateDefinitionDto : public FZStructDefinitionDto
{
	GENERATED_BODY()
	
	// @TODO
};

USTRUCT()
struct FZUnrealFieldManifestDto
{
	GENERATED_BODY()

	UPROPERTY()
	FName ModuleName;

	UPROPERTY()
	TArray<FZEnumDefinitionDto> Enums;
	
	UPROPERTY()
	TArray<FZScriptStructDefinitionDto> Structs;

	UPROPERTY()
	TArray<FZClassDefinitionDto> Classes;

	UPROPERTY()
	TArray<FZInterfaceDefinitionDto> Interfaces;

	UPROPERTY()
	TArray<FZDelegateDefinitionDto> Delegates;
};


