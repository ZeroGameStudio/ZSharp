// Copyright Zero Games. All Rights Reserved.


#include "ZCall/Container/ZSelfDescriptiveScriptArray.h"

ZSharp::FZSelfDescriptiveScriptArray::FZSelfDescriptiveScriptArray(const FProperty* elementProperty)
	: ElementProperty(elementProperty)
	, ElementPropertyVisitor(IZPropertyVisitor::Create(elementProperty))
	, UnderlyingInstance(new FScriptArray)
	, bOwning(true)
{
}

ZSharp::FZSelfDescriptiveScriptArray::FZSelfDescriptiveScriptArray(const FProperty* elementProperty, FScriptArray* underlyingInstance)
	: ElementProperty(elementProperty)
	, ElementPropertyVisitor(IZPropertyVisitor::Create(elementProperty))
	, UnderlyingInstance(underlyingInstance)
	, bOwning(false)
{
	GCRoot = TStrongObjectPtr { elementProperty->GetOwnerStruct() };
}

ZSharp::FZSelfDescriptiveScriptArray::~FZSelfDescriptiveScriptArray()
{
	if (bOwning)
	{
		// If ElementType is not trivially destructible, we need to destroy them manually.
		if (!ElementProperty->HasAnyPropertyFlags(CPF_IsPlainOldData | CPF_NoDestructor))
		{
			FScriptArrayHelper helper = GetHelper();
			const int32 num = helper.Num();
			for (int32 i = 0; i < num; ++i)
			{
				ElementProperty->DestroyValue(helper.GetElementPtr(i));
			}
		}
		
		delete UnderlyingInstance;
		delete ElementProperty;
	}
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
	ElementProperty = other.ElementProperty;
	UnderlyingInstance = other.UnderlyingInstance;
	bOwning = other.bOwning;
	other.bOwning = false;

	return *this;
}


