// Copyright Zero Games. All Rights Reserved.


#include "Reflection/Wrapper/ZSelfDescriptiveScriptMap.h"

ZSharp::FZSelfDescriptiveScriptMap::FZSelfDescriptiveScriptMap(const DescriptorType* descriptor)
	: Super(descriptor)
	, KeyPropertyVisitor(IZPropertyVisitor::Create(descriptor->Key))
	, ValuePropertyVisitor(IZPropertyVisitor::Create(descriptor->Value))
{
}

ZSharp::FZSelfDescriptiveScriptMap::FZSelfDescriptiveScriptMap(const DescriptorType* descriptor, UnderlyingInstanceType* underlyingInstance)
	: Super(descriptor, underlyingInstance)
	, KeyPropertyVisitor(IZPropertyVisitor::Create(descriptor->Key))
	, ValuePropertyVisitor(IZPropertyVisitor::Create(descriptor->Value))
{
	GCRoot = TStrongObjectPtr { Descriptor->Key->GetOwnerStruct() }; // Key and value have the same owner so we just check key's.
}

ZSharp::FZSelfDescriptiveScriptMap::FZSelfDescriptiveScriptMap(FZSelfDescriptiveScriptMap&& other) noexcept
	: Super(MoveTemp(other))
{
	KeyPropertyVisitor = MoveTemp(other.KeyPropertyVisitor);
	ValuePropertyVisitor = MoveTemp(other.ValuePropertyVisitor);
}

void ZSharp::FZSelfDescriptiveScriptMap::Add(const FZCallBufferSlot& src)
{
}

void ZSharp::FZSelfDescriptiveScriptMap::Remove(const FZCallBufferSlot& src)
{
}

bool ZSharp::FZSelfDescriptiveScriptMap::Find(const FZCallBufferSlot& src, FZCallBufferSlot& dest) const
{
	return true;
}

int32 ZSharp::FZSelfDescriptiveScriptMap::Num() const
{
	return 0;
}

ZSharp::FZSelfDescriptiveScriptMap& ZSharp::FZSelfDescriptiveScriptMap::operator=(FZSelfDescriptiveScriptMap&& other) noexcept
{
	Super::operator=(MoveTemp(other));
	
	KeyPropertyVisitor = MoveTemp(other.KeyPropertyVisitor);
	ValuePropertyVisitor = MoveTemp(other.ValuePropertyVisitor);

	return *this;
}

void ZSharp::FZSelfDescriptiveScriptMap::DeleteUnderlyingInstance()
{
	if (!Num())
	{
		return;
	}
	
	// If ElementType is not trivially destructible, we need to destroy them manually.
	const bool shouldDestroyKeys = !(Descriptor->Key->PropertyFlags & (CPF_IsPlainOldData | CPF_NoDestructor));
	const bool shouldDestroyValues = !(Descriptor->Value->PropertyFlags & (CPF_IsPlainOldData | CPF_NoDestructor));
	if (shouldDestroyKeys || shouldDestroyValues)
	{
		FScriptMapHelper helper = GetHelper();
		const uint32 stride  = helper.MapLayout.SetLayout.Size;
		uint8* ptr = helper.GetPairPtr(0);
		int32 num = helper.Num();
		for (int32 i = 0; num; ++i, ptr += stride)
		{
			if (helper.IsValidIndex(i))
			{
				if (shouldDestroyKeys)
				{
					Descriptor->Key->DestroyValue_InContainer(ptr);
				}
				if (shouldDestroyValues)
				{
					Descriptor->Value->DestroyValue_InContainer(ptr);
				}

				--num;
			}
		}
	}
		
	delete UnderlyingInstance;
}

void ZSharp::FZSelfDescriptiveScriptMap::DeleteDescriptor()
{
	delete Descriptor->Key;
	delete Descriptor->Value;
	delete Descriptor;
}


