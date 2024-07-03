// Copyright Zero Games. All Rights Reserved.


#include "IZPropertyVisitor.h"

#include "ZObjectPropertyVisitor.h"
#include "ZPrimitivePropertyVisitor.h"
#include "ZStructPropertyVisitor.h"

TUniquePtr<ZSharp::IZPropertyVisitor> ZSharp::IZPropertyVisitor::Create(const FProperty* prop)
{
	if (prop->IsA<FNumericProperty>() || prop->IsA<FBoolProperty>() || prop->IsA<FEnumProperty>())
	{
		return MakeUnique<FZPrimitivePropertyVisitor>(prop);
	}
	if (const auto objectProp = CastField<FObjectProperty>(prop))
	{
		return MakeUnique<FZObjectPropertyVisitor>(objectProp);
	}
	if (const auto structProp = CastField<FStructProperty>(prop))
	{
		return MakeUnique<FZStructPropertyVisitor>(structProp);
	}

	checkNoEntry();
	return nullptr;
}


