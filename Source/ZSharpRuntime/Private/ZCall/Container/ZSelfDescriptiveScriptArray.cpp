// Copyright Zero Games. All Rights Reserved.


#include "..\..\..\Public\ZCall\Container\ZSelfDescriptiveScriptArray.h"

ZSharp::FZSelfDescriptiveScriptArray::FZSelfDescriptiveScriptArray(const FProperty* elementProperty)
	: ElementProperty(elementProperty)
	, UnderlyingInstance(new FScriptArray)
	, bOwning(true)
{
}

ZSharp::FZSelfDescriptiveScriptArray::FZSelfDescriptiveScriptArray(const FProperty* elementProperty, FScriptArray* underlyingInstance, bool owning)
	: ElementProperty(elementProperty)
	, UnderlyingInstance(underlyingInstance)
	, bOwning(owning)
{
	if (!owning)
	{
		GCRoot = TStrongObjectPtr { elementProperty->GetOwnerStruct() };
	}
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

void* ZSharp::FZSelfDescriptiveScriptArray::Get(int32 index) const
{
	FScriptArrayHelper helper = GetHelper();
	return helper.GetElementPtr(index);
}

void ZSharp::FZSelfDescriptiveScriptArray::Set(int32 index, const void* item)
{
	FScriptArrayHelper helper = GetHelper();
	ElementProperty->CopyCompleteValue(helper.GetElementPtr(index), item);
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


