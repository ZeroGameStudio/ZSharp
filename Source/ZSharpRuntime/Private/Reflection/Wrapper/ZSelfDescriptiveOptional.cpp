// Copyright Zero Games. All Rights Reserved.

#include "Reflection/Wrapper/ZSelfDescriptiveOptional.h"

#include "NativePropertyDescriptorCheckMacros.h"
#include "Misc/ZUnmanagedHeapGCHelper.h"

ZSharp::FZSelfDescriptiveOptional::FZSelfDescriptiveOptional(const DescriptorType* descriptor, bool ownsDescriptor)
	: Super(descriptor, ownsDescriptor)
	, Helper(new FOptionalProperty(nullptr, {}, RF_Public | RF_MarkAsNative | RF_Transient))
	, UnderlyingPropertyVisitor(IZPropertyVisitor::Create(descriptor))
{
#if DO_CHECK
	if (ownsDescriptor)
	{
		const FProperty* property = descriptor;
		CHECK_NATIVE_PROPERTY_DESCRIPTOR();
	}
#endif

	class : public FArchive
	{
		virtual FArchive& operator<<(FObjectPtr& Value) override { return *this; }
	} ar;
	
	// FOptionalProperty owns it's ValueProperty, so we have to make a copy.
	auto duplicatedValueProperty = CastField<FProperty>(FField::Duplicate(descriptor, nullptr));
	Helper->SetValueProperty(duplicatedValueProperty);
	Helper->Link(ar);

	UnderlyingInstance = FMemory::Malloc(Helper->GetSize(), Helper->GetMinAlignment());
	Helper->InitializeValue(UnderlyingInstance);
}

ZSharp::FZSelfDescriptiveOptional::FZSelfDescriptiveOptional(const DescriptorType* descriptor, UnderlyingInstanceType* underlyingInstance)
	: Super(descriptor, underlyingInstance)
	, Helper(new FOptionalProperty(nullptr, {}, RF_Public | RF_MarkAsNative | RF_Transient))
	, UnderlyingPropertyVisitor(IZPropertyVisitor::Create(descriptor))
{
	// FOptionalProperty owns it's ValueProperty, so we have to make a copy.
	auto duplicatedValueProperty = CastField<FProperty>(FField::Duplicate(descriptor, nullptr));
	Helper->SetValueProperty(duplicatedValueProperty);
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
	if (bOwning)
	{
		Helper->DestroyValue(UnderlyingInstance);
		FMemory::Free(UnderlyingInstance);
	}
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

void ZSharp::FZSelfDescriptiveOptional::AddReferencedObjects(FReferenceCollector& collector)
{
	FZUnmanagedHeapGCHelper::CollectOptionalReference(collector, *Helper, UnderlyingInstance);
}

ZSharp::FZSelfDescriptiveOptional& ZSharp::FZSelfDescriptiveOptional::operator=(FZSelfDescriptiveOptional&& other) noexcept
{
	Super::operator=(MoveTemp(other));

	Helper = MoveTemp(other.Helper);
	UnderlyingPropertyVisitor = MoveTemp(other.UnderlyingPropertyVisitor);

	return *this;
}


