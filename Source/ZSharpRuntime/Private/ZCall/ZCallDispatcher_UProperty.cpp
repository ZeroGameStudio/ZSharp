// Copyright Zero Games. All Rights Reserved.


#include "ZCallDispatcher_UProperty.h"

#include "ZCall/ZCallBuffer.h"
#include "ZCall/ZCallBufferSlotEncoder.h"

ZSharp::FZCallDispatcher_UProperty::FZCallDispatcher_UProperty(const FString& name)
	: Name(name)
	, Property(nullptr)
	, bAvailable(false)
{
	Name.RightChop(4).Split(TEXT(":"), &ClassPath, &PropertyName, ESearchCase::CaseSensitive, ESearchDir::FromEnd);
}

int32 ZSharp::FZCallDispatcher_UProperty::Dispatch(FZCallBuffer* buffer) const
{
	if (!GCRoot.IsValid(true))
	{
		if (bAvailable = InvalidateCache(); !bAvailable)
		{
			return 1;
		}
	}

	if (!bAvailable)
	{
		return 2;
	}

	FZCallBuffer& buf = *buffer;
	UObject* self = TZCallBufferSlotEncoder<UObject*>::Decode(buf[0]);
	check(self);
	check(self->IsA(GCRoot.Get()));

	const bool write = buf[1].ReadBool();
	if (write)
	{
		Property->SetValue_InContainer(self, buf[2]);
	}
	else
	{
		Property->GetValue_InContainer(self, buf[2]);
	}
	
	return 0;
}

bool ZSharp::FZCallDispatcher_UProperty::InvalidateCache() const
{
	Property.Reset();
	
	GCRoot = LoadClass<UObject>(nullptr, *ClassPath);
	if (!GCRoot.IsValid(true))
	{
		return false;
	}

	const FProperty* prop = FindFProperty<FProperty>(GCRoot.Get(), *PropertyName);
	if (!prop)
	{
		return false;
	}

	Property = IZPropertyVisitor::Create(prop);
	if (!Property)
	{
		return false;
	}
	
	return true;
}


