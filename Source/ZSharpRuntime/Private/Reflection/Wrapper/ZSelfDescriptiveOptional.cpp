// Copyright Zero Games. All Rights Reserved.


#include "Reflection/Wrapper/ZSelfDescriptiveOptional.h"

ZSharp::FZSelfDescriptiveOptional::FZSelfDescriptiveOptional(const DescriptorType* descriptor)
	: Super(descriptor)
	, Helper(new FOptionalProperty(nullptr, {}, RF_Public | RF_MarkAsNative | RF_Transient))
	, UnderlyingPropertyVisitor(IZPropertyVisitor::Create(descriptor))
{
	Helper->SetValueProperty(const_cast<FProperty*>(descriptor));
	Helper->InitializeValue(UnderlyingInstance);
}

ZSharp::FZSelfDescriptiveOptional::FZSelfDescriptiveOptional(const DescriptorType* descriptor, UnderlyingInstanceType* underlyingInstance)
	: Super(descriptor, underlyingInstance)
	, Helper(new FOptionalProperty(nullptr, {}, RF_Public | RF_MarkAsNative | RF_Transient))
	, UnderlyingPropertyVisitor(IZPropertyVisitor::Create(descriptor))
{
	Helper->SetValueProperty(const_cast<FProperty*>(descriptor));
	GCRoot = TStrongObjectPtr { Descriptor->GetOwnerStruct() };
}

ZSharp::FZSelfDescriptiveOptional::FZSelfDescriptiveOptional(FZSelfDescriptiveOptional&& other) noexcept
	: Super(MoveTemp(other))
{
	Helper = MoveTemp(other.Helper);
	UnderlyingPropertyVisitor = MoveTemp(other.UnderlyingPropertyVisitor);
}

ZSharp::FZSelfDescriptiveOptional::~FZSelfDescriptiveOptional()
{
	Helper->DestroyValue(UnderlyingInstance);
	FMemory::Free(UnderlyingInstance);
}

bool ZSharp::FZSelfDescriptiveOptional::Get(FZCallBufferSlot& dest) const
{
	if (IsSet())
	{
		UnderlyingPropertyVisitor->GetValue(Helper->GetValuePointerForRead(UnderlyingInstance), dest);
		return true;
	}
	
	return false;
}

void ZSharp::FZSelfDescriptiveOptional::Set(const FZCallBufferSlot& src)
{
	UnderlyingPropertyVisitor->SetValue(Helper->MarkSetAndGetInitializedValuePointerToReplace(UnderlyingInstance), src);
}

void ZSharp::FZSelfDescriptiveOptional::Reset()
{
	Helper->MarkUnset(UnderlyingInstance);
}

bool ZSharp::FZSelfDescriptiveOptional::IsSet() const
{
	return Helper->IsSet(UnderlyingInstance);
}

ZSharp::FZSelfDescriptiveOptional& ZSharp::FZSelfDescriptiveOptional::operator=(FZSelfDescriptiveOptional&& other) noexcept
{
	Super::operator=(MoveTemp(other));

	Helper = MoveTemp(other.Helper);
	UnderlyingPropertyVisitor = MoveTemp(other.UnderlyingPropertyVisitor);

	return *this;
}

ZSharp::FZSelfDescriptiveOptional::UnderlyingInstanceType* ZSharp::FZSelfDescriptiveOptional::NewUnderlyingInstance(const DescriptorType* descriptor)
{
	// Here the helper is not available so we only allocate memory for the underlying instance.
	// Let our constructor to initialize it.
	return FMemory::Malloc(descriptor->GetSize(), descriptor->GetMinAlignment());
}


