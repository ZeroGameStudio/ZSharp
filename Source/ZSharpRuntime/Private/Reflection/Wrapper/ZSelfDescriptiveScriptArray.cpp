// Copyright Zero Games. All Rights Reserved.


#include "Reflection/Wrapper/ZSelfDescriptiveScriptArray.h"

ZSharp::FZSelfDescriptiveScriptArray::FZSelfDescriptiveScriptArray(const DescriptorType* descriptor)
	: Super(descriptor)
	, ElementPropertyVisitor(IZPropertyVisitor::Create(GetDescriptor()))
{
	GCRoot = TStrongObjectPtr { Descriptor->GetOwnerStruct() };
}

ZSharp::FZSelfDescriptiveScriptArray::FZSelfDescriptiveScriptArray(const DescriptorType* descriptor, UnderlyingInstanceType* underlyingInstance)
	: Super(descriptor, underlyingInstance)
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
	ElementPropertyVisitor->GetValue(src, dest);
}

void ZSharp::FZSelfDescriptiveScriptArray::Set(int32 index, const FZCallBufferSlot& src)
{
	FScriptArrayHelper helper = GetHelper();
	void* dest = helper.GetElementPtr(index);
	ElementPropertyVisitor->SetValue(dest, src);
}

int32 ZSharp::FZSelfDescriptiveScriptArray::Num() const
{
	FScriptArrayHelper helper = GetHelper();
	return helper.Num();
}

ZSharp::FZSelfDescriptiveScriptArray& ZSharp::FZSelfDescriptiveScriptArray::operator=(FZSelfDescriptiveScriptArray&& other) noexcept
{
	Super::operator=(MoveTemp(other));
	
	GCRoot = other.GCRoot;
	other.GCRoot.Reset();
	ElementPropertyVisitor = MoveTemp(other.ElementPropertyVisitor);

	return *this;
}

void ZSharp::FZSelfDescriptiveScriptArray::DeleteUnderlyingInstance()
{
	// If ElementType is not trivially destructible, we need to destroy them manually.
	if (!Descriptor->HasAnyPropertyFlags(CPF_IsPlainOldData | CPF_NoDestructor))
	{
		FScriptArrayHelper helper = GetHelper();
		const int32 num = helper.Num();
		for (int32 i = 0; i < num; ++i)
		{
			Descriptor->DestroyValue(helper.GetElementPtr(i));
		}
	}
		
	delete UnderlyingInstance;
}


