﻿// Copyright Zero Games. All Rights Reserved.


#include "Reflection/Wrapper/ZSelfDescriptiveScriptArray.h"

ZSharp::FZSelfDescriptiveScriptArray::FZSelfDescriptiveScriptArray(const DescriptorType* descriptor)
	: Super(descriptor)
	, ElementPropertyVisitor(IZPropertyVisitor::Create(descriptor))
{

}

ZSharp::FZSelfDescriptiveScriptArray::FZSelfDescriptiveScriptArray(const DescriptorType* descriptor, UnderlyingInstanceType* underlyingInstance)
	: Super(descriptor, underlyingInstance)
	, ElementPropertyVisitor(IZPropertyVisitor::Create(descriptor))
{
	GCRoot = TStrongObjectPtr { Descriptor->GetOwnerStruct() };
}

ZSharp::FZSelfDescriptiveScriptArray::FZSelfDescriptiveScriptArray(FZSelfDescriptiveScriptArray&& other) noexcept
	: Super(MoveTemp(other))
{
	ElementPropertyVisitor = MoveTemp(other.ElementPropertyVisitor);
}

void ZSharp::FZSelfDescriptiveScriptArray::InsertAt(int32 index)
{
	FScriptArrayHelper helper = GetHelper();
	helper.InsertValues(index);
}

void ZSharp::FZSelfDescriptiveScriptArray::RemoveAt(int32 index)
{
	FScriptArrayHelper helper = GetHelper();
	helper.RemoveValues(index);
}

void ZSharp::FZSelfDescriptiveScriptArray::Get(int32 index, FZCallBufferSlot& dest) const
{
	FScriptArrayHelper helper = GetHelper();
	const void* src = helper.GetElementPtr(index);
	ElementPropertyVisitor->GetValue(src, dest); // Always get copy instead of reference for memory safety.
}

void ZSharp::FZSelfDescriptiveScriptArray::Set(int32 index, const FZCallBufferSlot& src)
{
	FScriptArrayHelper helper = GetHelper();
	void* dest = helper.GetElementPtr(index);
	ElementPropertyVisitor->SetValue(dest, src);
}

void ZSharp::FZSelfDescriptiveScriptArray::Clear()
{
	FScriptArrayHelper helper = GetHelper();
	helper.EmptyValues();
}

int32 ZSharp::FZSelfDescriptiveScriptArray::Num() const
{
	FScriptArrayHelper helper = GetHelper();
	return helper.Num();
}

ZSharp::FZSelfDescriptiveScriptArray& ZSharp::FZSelfDescriptiveScriptArray::operator=(FZSelfDescriptiveScriptArray&& other) noexcept
{
	Super::operator=(MoveTemp(other));
	
	ElementPropertyVisitor = MoveTemp(other.ElementPropertyVisitor);

	return *this;
}

void ZSharp::FZSelfDescriptiveScriptArray::DeleteUnderlyingInstance()
{
	if (!Num())
	{
		return;
	}
	
	// If ElementType is not trivially destructible, we need to destroy them manually.
	if (!Descriptor->HasAnyPropertyFlags(CPF_IsPlainOldData | CPF_NoDestructor))
	{
		FScriptArrayHelper helper = GetHelper();
		const uint32 stride = Descriptor->GetSize();
		uint8* ptr = helper.GetElementPtr(0);
		const int32 num = helper.Num();
		for (int32 i = 0; i < num; ++i, ptr += stride)
		{
			Descriptor->DestroyValue_InContainer(ptr);
		}
	}
		
	delete UnderlyingInstance;
}


