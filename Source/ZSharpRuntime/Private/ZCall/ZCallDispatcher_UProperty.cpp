// Copyright Zero Games. All Rights Reserved.


#include "ZCallDispatcher_UProperty.h"

#include "ZCall/ZCallBuffer.h"
#include "ZCall/ZCallBufferSlotEncoder.h"

ZSharp::FZCallDispatcher_UProperty::FZCallDispatcher_UProperty(const FString& name)
	: Name(name)
	, Property(nullptr)
	, bAvailable(false)
{
	Name.RightChop(4).Split(TEXT(":"), &StructPath, &PropertyName, ESearchCase::CaseSensitive, ESearchDir::FromEnd);
}

ZSharp::EZCallErrorCode ZSharp::FZCallDispatcher_UProperty::Dispatch(FZCallBuffer* buffer) const
{
	if (!GCRoot.IsValid(true))
	{
		if (bAvailable = InvalidateCache(); !bAvailable)
		{
			return EZCallErrorCode::DispatcherError;
		}
	}

	if (!bAvailable)
	{
		return EZCallErrorCode::DispatcherError;
	}

	FZCallBuffer& buf = *buffer;
	void* self;
	if (const UClass* cls = Cast<UClass>(GCRoot.Get()))
	{
		UObject* typedSelf = TZCallBufferSlotEncoder<UObject*>::Decode(buf[0]);
		check(typedSelf);
		check(typedSelf->IsA(cls));
		self = typedSelf;
	}
	else
	{
		const FZSelfDescriptiveScriptStruct* sdss = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UScriptStruct>().Conjugate(buf[0].ReadConjugate());
		self = sdss ? sdss->GetUnderlyingInstance() : nullptr;
	}

	const bool write = buf[1].ReadBool();
	const int32 index = buf[2].ReadInt32();
	if (write)
	{
		Property->SetValue_InContainer(self, buf[3], index);
	}
	else
	{
		Property->GetRef_InContainer(self, buf[3], index);
	}
	
	return EZCallErrorCode::Succeed;
}

bool ZSharp::FZCallDispatcher_UProperty::InvalidateCache() const
{
	Property.Reset();
	
	GCRoot = LoadObject<UStruct>(nullptr, *StructPath);
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


