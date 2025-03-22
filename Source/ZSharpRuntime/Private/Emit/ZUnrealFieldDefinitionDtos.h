// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZUnrealFieldDefinitionDtos.generated.h"

static_assert(std::is_same_v<__underlying_type(EObjectFlags), int32>);
static_assert(std::is_same_v<__underlying_type(EPropertyFlags), uint64>);
static_assert(std::is_same_v<__underlying_type(EFunctionFlags), uint32>);
static_assert(std::is_same_v<__underlying_type(EClassFlags), int32>);
static_assert(std::is_same_v<__underlying_type(EClassCastFlags), uint64>);
static_assert(std::is_same_v<__underlying_type(EStructFlags), int32>);

USTRUCT(meta = (ZSharpNoExport))
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

USTRUCT(meta = (ZSharpNoExport))
struct FZSimplePropertyDefinitionDto
{
	GENERATED_BODY()

	UPROPERTY()
	uint8 Type = 0;

	UPROPERTY()
	uint8 EnumUnderlyingType = 0;

	UPROPERTY()
	uint64 PropertyFlags = 0;

	UPROPERTY()
	FName DescriptorFieldPath;
};

USTRUCT(meta = (ZSharpNoExport))
struct FZPropertyDefinitionDto : public FZFieldDefinitionDto
{
	GENERATED_BODY()

	UPROPERTY()
	uint8 Type = 0;
	
	UPROPERTY()
	uint8 EnumUnderlyingType = 0;

	UPROPERTY()
	uint64 PropertyFlags = 0;

	UPROPERTY()
	FName DescriptorFieldPath;

	UPROPERTY()
	FName RepNotifyName;

	UPROPERTY()
	int32 RepCondition = 0;

	UPROPERTY()
	int32 RepNotifyCondition = 0;

	UPROPERTY()
	bool IsRepPushBased = false;
	
	UPROPERTY()
	FZSimplePropertyDefinitionDto InnerProperty;

	UPROPERTY()
	FZSimplePropertyDefinitionDto OuterProperty;
	
};

USTRUCT(meta = (ZSharpNoExport))
struct FZStructDefinitionDto_PropertyDefault
{
	GENERATED_BODY()

	UPROPERTY()
	FString PropertyChain;

	UPROPERTY()
	FString Buffer;
};

USTRUCT(meta = (ZSharpNoExport))
struct FZStructDefinitionDto : public FZFieldDefinitionDto
{
	GENERATED_BODY()

	UPROPERTY()
	FName SuperPath;

	UPROPERTY()
	TArray<FZPropertyDefinitionDto> Properties;

	UPROPERTY()
	TArray<FZStructDefinitionDto_PropertyDefault> PropertyDefaults;
};

USTRUCT(meta = (ZSharpNoExport))
struct FZFunctionDefinitionDto : public FZStructDefinitionDto
{
	GENERATED_BODY()

	UPROPERTY()
	bool bIsEventOverride = false;

	UPROPERTY()
	uint32 FunctionFlags = 0;

	UPROPERTY()
	FString ZCallName;

	UPROPERTY()
	FString ValidateZCallName;

	UPROPERTY()
	uint16 RpcId = 0;

	UPROPERTY()
	uint16 RpcResponseId = 0;

	UPROPERTY()
	FName CustomThunkName;
};

USTRUCT(meta = (ZSharpNoExport))
struct FZEnumFieldDefinitionDto : public FZFieldDefinitionDto
{
	GENERATED_BODY()
	
	UPROPERTY()
	int64 Value = 0;
};

USTRUCT(meta = (ZSharpNoExport))
struct FZEnumDefinitionDto : public FZFieldDefinitionDto
{
	GENERATED_BODY()

	UPROPERTY()
	uint8 UnderlyingType = 0;
	
	UPROPERTY()
	uint8 EnumFlags = 0;

	UPROPERTY()
	TArray<FZEnumFieldDefinitionDto> Fields;
};

USTRUCT(meta = (ZSharpNoExport))
struct FZScriptStructDefinitionDto : public FZStructDefinitionDto
{
	GENERATED_BODY()
	
	UPROPERTY()
	int32 StructFlags = 0;
};

USTRUCT(meta = (ZSharpNoExport))
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

USTRUCT(meta = (ZSharpNoExport))
struct FZClassDefinitionDto_DefaultSubobjectOverride
{
	GENERATED_BODY()

	UPROPERTY()
	FName Name;

	UPROPERTY()
	FName ClassPath;
};

USTRUCT(meta = (ZSharpNoExport))
struct FZDelegateDefinitionDto : public FZStructDefinitionDto
{
	GENERATED_BODY()
	
	UPROPERTY()
	uint8 DelegateType = 0;

	UPROPERTY()
	FName OuterClassName;
};

USTRUCT(meta = (ZSharpNoExport))
struct FZInterfaceDefinitionDto : public FZStructDefinitionDto
{
	GENERATED_BODY()
	
	// @TODO
};

USTRUCT(meta = (ZSharpNoExport))
struct FZClassDefinitionDto : public FZStructDefinitionDto
{
	GENERATED_BODY()

	UPROPERTY()
	bool bConstructor = false;

	UPROPERTY()
	bool bContextual = false;

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
	TArray<FZClassDefinitionDto_DefaultSubobject> DefaultSubobjects;

	UPROPERTY()
	TArray<FZClassDefinitionDto_DefaultSubobjectOverride> DefaultSubobjectOverrides;

	UPROPERTY()
	TArray<FName> FieldNotifies;
};

USTRUCT(meta = (ZSharpNoExport))
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
	TArray<FZDelegateDefinitionDto> Delegates;

	UPROPERTY()
	TArray<FZInterfaceDefinitionDto> Interfaces;

	UPROPERTY()
	TArray<FZClassDefinitionDto> Classes;
};


