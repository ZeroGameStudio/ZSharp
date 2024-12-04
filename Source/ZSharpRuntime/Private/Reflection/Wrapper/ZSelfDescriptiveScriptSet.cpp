// Copyright Zero Games. All Rights Reserved.


#include "Reflection/Wrapper/ZSelfDescriptiveScriptSet.h"

#include "NativePropertyDescriptorCheckMacros.h"

ZSharp::FZSelfDescriptiveScriptSet::FZSelfDescriptiveScriptSet(const DescriptorType* descriptor, bool ownsDescriptor)
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

ZSharp::FZSelfDescriptiveScriptSet::FZSelfDescriptiveScriptSet(const DescriptorType* descriptor, UnderlyingInstanceType* underlyingInstance)
	: Super(descriptor, underlyingInstance)
	, ElementPropertyVisitor(IZPropertyVisitor::Create(descriptor))
{
	GCRoot = TStrongObjectPtr { Descriptor->GetOwnerStruct() };
}

ZSharp::FZSelfDescriptiveScriptSet::FZSelfDescriptiveScriptSet(FZSelfDescriptiveScriptSet&& other) noexcept
	: Super(MoveTemp(other))
{
	ElementPropertyVisitor = MoveTemp(other.ElementPropertyVisitor);
}

bool ZSharp::FZSelfDescriptiveScriptSet::Add(const FZCallBufferSlot& src)
{
	const bool alreadyExists = Contains(src);
	if (!alreadyExists)
	{
		FScriptSetHelper helper = GetHelper();
		void* data = FMemory_Alloca(Descriptor->GetSize());
		ElementPropertyVisitor->InitializeValue(data);
		ElementPropertyVisitor->SetValue(data, src);
		helper.AddElement(data);
	}

	return !alreadyExists;
}

bool ZSharp::FZSelfDescriptiveScriptSet::Remove(const FZCallBufferSlot& src)
{
	FScriptSetHelper helper = GetHelper();
	void* data = FMemory_Alloca(Descriptor->GetSize());
	ElementPropertyVisitor->InitializeValue(data);
	ElementPropertyVisitor->SetValue(data, src);
	return helper.RemoveElement(data);
}

void ZSharp::FZSelfDescriptiveScriptSet::Clear()
{
	FScriptSetHelper helper = GetHelper();
	helper.EmptyElements();
}

bool ZSharp::FZSelfDescriptiveScriptSet::Contains(const FZCallBufferSlot& src) const
{
	FScriptSetHelper helper = GetHelper();
	void* data = FMemory_Alloca(Descriptor->GetSize());
	ElementPropertyVisitor->InitializeValue(data);
	ElementPropertyVisitor->SetValue(data, src);
	return helper.FindElementIndexFromHash(data) != INDEX_NONE;
}

int32 ZSharp::FZSelfDescriptiveScriptSet::Num() const
{
	FScriptSetHelper helper = GetHelper();
	return helper.Num();
}

void ZSharp::FZSelfDescriptiveScriptSet::Get(FScriptSetHelper::FIterator it, FZCallBufferSlot& dest) const
{
	FScriptSetHelper helper = GetHelper();
	void* src = helper.GetElementPtr(it);
	ElementPropertyVisitor->GetValue(src, dest);
}

ZSharp::FZSelfDescriptiveScriptSet& ZSharp::FZSelfDescriptiveScriptSet::operator=(FZSelfDescriptiveScriptSet&& other) noexcept
{
	Super::operator=(MoveTemp(other));
	
	ElementPropertyVisitor = MoveTemp(other.ElementPropertyVisitor);

	return *this;
}

void ZSharp::FZSelfDescriptiveScriptSet::DeleteUnderlyingInstance()
{
	if (!Num())
	{
		return;
	}
	
	// If ElementType is not trivially destructible, we need to destroy them manually.
	if (!Descriptor->HasAnyPropertyFlags(CPF_IsPlainOldData | CPF_NoDestructor))
	{
		FScriptSetHelper helper = GetHelper();
		const uint32 stride = helper.SetLayout.Size;
		uint8* ptr = helper.GetElementPtr(0);
		int32 num = helper.Num();
		for (int32 i = 0; num; ++i, ptr += stride)
		{
			if (helper.IsValidIndex(i))
			{
				Descriptor->DestroyValue_InContainer(ptr);
				--num;
			}
		}
	}
		
	delete UnderlyingInstance;
}
