// Copyright Zero Games. All Rights Reserved.


#include "Reflection/Wrapper/ZSelfDescriptiveScriptMap.h"

#include "NativePropertyDescriptorCheckMacros.h"

ZSharp::FZSelfDescriptiveScriptMap::FZSelfDescriptiveScriptMap(const DescriptorType* descriptor, bool ownsDescriptor)
	: Super(descriptor, ownsDescriptor)
	, KeyPropertyVisitor(IZPropertyVisitor::Create(descriptor->Key))
	, ValuePropertyVisitor(IZPropertyVisitor::Create(descriptor->Value))
{
#if DO_CHECK
	if (ownsDescriptor)
	{
		{
			const FProperty* property = descriptor->Key;
			CHECK_NATIVE_PROPERTY_DESCRIPTOR();
		}

		{
			const FProperty* property = descriptor->Value;
			CHECK_NATIVE_PROPERTY_DESCRIPTOR();
		}
	}
#endif
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

bool ZSharp::FZSelfDescriptiveScriptMap::TryAdd(const FZCallBufferSlot& key, const FZCallBufferSlot& value)
{
	if (Contains(key))
	{
		return false;
	}
	
	Set(key, value);
	return true;
}

bool ZSharp::FZSelfDescriptiveScriptMap::Remove(const FZCallBufferSlot& key)
{
	FScriptMapHelper helper = GetHelper();
	void* keyData = FMemory_Alloca(Descriptor->Key->GetSize());
	KeyPropertyVisitor->InitializeValue(keyData);
	KeyPropertyVisitor->SetValue(keyData, key);
	
	return helper.RemovePair(keyData);
}

void ZSharp::FZSelfDescriptiveScriptMap::Clear()
{
	FScriptMapHelper helper = GetHelper();
	helper.EmptyValues();
}

bool ZSharp::FZSelfDescriptiveScriptMap::Contains(const FZCallBufferSlot& key) const
{
	FScriptMapHelper helper = GetHelper();
	void* keyData = FMemory_Alloca(Descriptor->Key->GetSize());
	KeyPropertyVisitor->InitializeValue(keyData);
	KeyPropertyVisitor->SetValue(keyData, key);
	
	return !!helper.FindValueFromHash(keyData);
}

bool ZSharp::FZSelfDescriptiveScriptMap::Find(const FZCallBufferSlot& key, FZCallBufferSlot& value) const
{
	FScriptMapHelper helper = GetHelper();
	void* keyData = FMemory_Alloca(Descriptor->Key->GetSize());
	KeyPropertyVisitor->InitializeValue(keyData);
	KeyPropertyVisitor->SetValue(keyData, key);
	
	if (void* valueData = helper.FindValueFromHash(keyData))
	{
		ValuePropertyVisitor->GetValue(valueData, value);
		return true;
	}
	else
	{
		return false;
	}
}

void ZSharp::FZSelfDescriptiveScriptMap::Set(const FZCallBufferSlot& key, const FZCallBufferSlot& value)
{
	FScriptMapHelper helper = GetHelper();
	void* keyData = FMemory_Alloca(Descriptor->Key->GetSize());
	KeyPropertyVisitor->InitializeValue(keyData);
	KeyPropertyVisitor->SetValue(keyData, key);
	void* valueData = FMemory_Alloca(Descriptor->Value->GetSize());
	ValuePropertyVisitor->InitializeValue(valueData);
	ValuePropertyVisitor->SetValue(valueData, value);

	helper.AddPair(keyData, valueData);
}

int32 ZSharp::FZSelfDescriptiveScriptMap::Num() const
{
	FScriptMapHelper helper = GetHelper();
	return helper.Num();
}

void ZSharp::FZSelfDescriptiveScriptMap::Get(FScriptMapHelper::FIterator it, FZCallBufferSlot& key, FZCallBufferSlot& value) const
{
	FScriptMapHelper helper = GetHelper();
	void* src = helper.GetPairPtr(it);
	KeyPropertyVisitor->GetValue_InContainer(src, key, 0);
	ValuePropertyVisitor->GetValue_InContainer(src, value, 0);
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
}

void ZSharp::FZSelfDescriptiveScriptMap::Finalize()
{
	delete Descriptor;
}


