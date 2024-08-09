// Copyright Zero Games. All Rights Reserved.


#include "Reflection/Wrapper/ZSelfDescriptiveScriptSet.h"

ZSharp::FZSelfDescriptiveScriptSet::FZSelfDescriptiveScriptSet(const DescriptorType* descriptor)
	: Super(descriptor)
	, ElementPropertyVisitor(IZPropertyVisitor::Create(descriptor))
{
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

void ZSharp::FZSelfDescriptiveScriptSet::Add(const FZCallBufferSlot& src)
{
	FScriptSetHelper helper = GetHelper();
	void* data = FMemory_Alloca_Aligned(Descriptor->GetSize(), Descriptor->GetMinAlignment());
	ElementPropertyVisitor->SetValue(data, src);
	helper.AddElement(data);
}

void ZSharp::FZSelfDescriptiveScriptSet::Remove(const FZCallBufferSlot& src)
{
	FScriptSetHelper helper = GetHelper();
	void* data = FMemory_Alloca_Aligned(Descriptor->GetSize(), Descriptor->GetMinAlignment());
	ElementPropertyVisitor->SetValue(data, src);
	helper.RemoveElement(data);
}

bool ZSharp::FZSelfDescriptiveScriptSet::Contains(const FZCallBufferSlot& src) const
{
	FScriptSetHelper helper = GetHelper();
	void* data = FMemory_Alloca_Aligned(Descriptor->GetSize(), Descriptor->GetMinAlignment());
	ElementPropertyVisitor->SetValue(data, src);
	
	return helper.FindElementIndexFromHash(data) != INDEX_NONE;
}

int32 ZSharp::FZSelfDescriptiveScriptSet::Num() const
{
	FScriptSetHelper helper = GetHelper();
	return helper.Num();
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
