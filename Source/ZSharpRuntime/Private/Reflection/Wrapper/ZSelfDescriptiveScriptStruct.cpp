﻿// Copyright Zero Games. All Rights Reserved.


#include "Reflection/Wrapper/ZSelfDescriptiveScriptStruct.h"

ZSharp::FZSelfDescriptiveScriptStruct::FZSelfDescriptiveScriptStruct(const UScriptStruct* descriptor)
	: Descriptor(descriptor)
	, UnderlyingInstance(FMemory::Malloc(descriptor->GetStructureSize(), descriptor->GetMinAlignment()))
	, bOwning(true)
{
	descriptor->InitializeStruct(UnderlyingInstance);
}

ZSharp::FZSelfDescriptiveScriptStruct::FZSelfDescriptiveScriptStruct(const UScriptStruct* descriptor, void* underlyingInstance)
	: Descriptor(descriptor)
	, UnderlyingInstance(underlyingInstance)
	, bOwning(false)
{
}

ZSharp::FZSelfDescriptiveScriptStruct::~FZSelfDescriptiveScriptStruct()
{
	if (bOwning)
	{
		Descriptor->DestroyStruct(UnderlyingInstance);
		FMemory::Free(UnderlyingInstance);
	}
}

ZSharp::FZSelfDescriptiveScriptStruct& ZSharp::FZSelfDescriptiveScriptStruct::operator=(FZSelfDescriptiveScriptStruct&& other) noexcept
{
	Descriptor = other.Descriptor;
	UnderlyingInstance = other.UnderlyingInstance;
	bOwning = other.bOwning;
	other.bOwning = false;

	return *this;
}