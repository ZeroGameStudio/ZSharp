// Copyright Zero Games. All Rights Reserved.

#include "Reflection/Wrapper/ZSelfDescriptiveScriptArray.h"

#include "NativePropertyDescriptorCheckMacros.h"

ZSharp::FZSelfDescriptiveScriptArray::FZSelfDescriptiveScriptArray(const DescriptorType* descriptor, bool ownsDescriptor)
	: Super(descriptor, ownsDescriptor)
	, ElementPropertyVisitor(IZPropertyVisitor::Create(descriptor))
{
#if DO_CHECK
	if (ownsDescriptor)
	{
		const FProperty* property = descriptor;
		CHECK_NATIVE_PROPERTY_DESCRIPTOR();
	}
#endif
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

void ZSharp::FZSelfDescriptiveScriptArray::Insert(int32 index, const FZCallBufferSlot& src)
{
	FScriptArrayHelper helper = GetHelper();
	helper.InsertValues(index);
	void* dest = helper.GetElementPtr(index);
	ElementPropertyVisitor->SetValue(dest, src);
}

void ZSharp::FZSelfDescriptiveScriptArray::InsertAt(int32 index)
{
	FScriptArrayHelper helper = GetHelper();
	helper.InsertValues(index);
}

bool ZSharp::FZSelfDescriptiveScriptArray::Remove(const FZCallBufferSlot& src)
{
	if (int32 index = IndexOf(src); index >= 0)
	{
		RemoveAt(index);
		return true;
	}

	return false;
}

void ZSharp::FZSelfDescriptiveScriptArray::RemoveAt(int32 index)
{
	FScriptArrayHelper helper = GetHelper();
	helper.RemoveValues(index);
}

void ZSharp::FZSelfDescriptiveScriptArray::Clear()
{
	FScriptArrayHelper helper = GetHelper();
	helper.EmptyValues();
}

int32 ZSharp::FZSelfDescriptiveScriptArray::IndexOf(const FZCallBufferSlot& src)
{
	FScriptArrayHelper helper = GetHelper();
	const uint32 stride = Descriptor->GetSize();
	uint8* ptr = helper.GetElementPtr(0);
	const int32 num = helper.Num();
	void* value = FMemory_Alloca(stride);
	ElementPropertyVisitor->InitializeValue(value);
	ElementPropertyVisitor->SetValue(value, src);
	for (int32 i = 0; i < num; ++i, ptr += stride)
	{
		if (Descriptor->Identical(ptr, value))
		{
			return i;
		}
	}

	return INDEX_NONE;
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

int32 ZSharp::FZSelfDescriptiveScriptArray::Num() const
{
	FScriptArrayHelper helper = GetHelper();
	return helper.Num();
}

void ZSharp::FZSelfDescriptiveScriptArray::AddReferencedObjects(FReferenceCollector& collector)
{
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


