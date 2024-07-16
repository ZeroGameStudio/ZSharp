// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZFullyExportedTypeName.h"

#include "ZFullyExportedTypeNameDto.generated.h"

USTRUCT()
struct FZSingleFullyExportedTypeNameDto
{
	GENERATED_BODY()

	UPROPERTY()
	FString Namespace;

	UPROPERTY()
	FString Name;

	UPROPERTY()
	bool bNullable = false;

	FZSingleFullyExportedTypeNameDto& operator=(const ZSharp::FZSingleFullyExportedTypeName& other)
	{
		Namespace = other.Namespace;
		Name = other.Name;
		bNullable = other.bNullable;

		return *this;
	}
	
};

USTRUCT()
struct FZSimpleFullyExportedTypeNameDto
{
	GENERATED_BODY()

	UPROPERTY()
	FString Namespace;

	UPROPERTY()
	FString Name;

	UPROPERTY()
	bool bNullable = false;

	UPROPERTY()
	FZSingleFullyExportedTypeNameDto Inner;

	FZSimpleFullyExportedTypeNameDto& operator=(const ZSharp::FZSimpleFullyExportedTypeName& other)
	{
		Namespace = other.Namespace;
		Name = other.Name;
		bNullable = other.bNullable;
		Inner = other.Inner;

		return *this;
	}
	
};

USTRUCT()
struct FZFullyExportedTypeNameDto
{
	GENERATED_BODY()
	
	UPROPERTY()
	FString Namespace;

	UPROPERTY()
	FString Name;

	UPROPERTY()
	bool bNullable = false;

	UPROPERTY()
	FZSimpleFullyExportedTypeNameDto Inner;

	UPROPERTY()
	FZSimpleFullyExportedTypeNameDto Outer;

	FZFullyExportedTypeNameDto& operator=(const ZSharp::FZFullyExportedTypeName& other)
	{
		Namespace = other.Namespace;
		Name = other.Name;
		bNullable = other.bNullable;
		Inner = other.Inner;
		Outer = other.Outer;
		
		return *this;
	}
};


