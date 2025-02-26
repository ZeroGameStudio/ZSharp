// Copyright Zero Games. All Rights Reserved.

#include "ZUEnumUnderlyingTypeLookup.h"

#include "ZSharpExportSettings.h"

ZSharp::FZUEnumUnderlyingTypeLookup& ZSharp::FZUEnumUnderlyingTypeLookup::Get()
{
	static FZUEnumUnderlyingTypeLookup GInstance;

	return GInstance;
}

void ZSharp::FZUEnumUnderlyingTypeLookup::InvalidateCache()
{
	static const TMap<FFieldClass*, FString> GNameMap
	{
		{ FByteProperty::StaticClass(), "uint8" },
		{ FUInt16Property::StaticClass(), "uint16" },
		{ FUInt32Property::StaticClass(), "uint32" },
		{ FUInt64Property::StaticClass(), "uint64" },
		{ FInt8Property::StaticClass(), "int8" },
		{ FInt16Property::StaticClass(), "int16" },
		{ FIntProperty::StaticClass(), "int32" },
		{ FInt64Property::StaticClass(), "int64" },
	};
	
	Cache.Empty();

	// Traverse all UStruct object to loot enum property underlying type.
	for (TObjectIterator<UStruct> it; it; ++it)
	{
		const UStruct* strct = *it;
		if (!strct->IsNative())
		{
			continue;
		}
		
		for (TFieldIterator<FProperty> pit(strct, EFieldIteratorFlags::ExcludeSuper); pit; ++pit)
		{
			FProperty* property = *pit;
			const UEnum* enm = nullptr;
			const FFieldClass* underlyingPropertyClass = nullptr;
			if (auto enumProperty = CastField<FEnumProperty>(property))
			{
				enm = enumProperty->GetEnum();
				underlyingPropertyClass = enumProperty->GetUnderlyingProperty()->GetClass();
			}
			else if (GetDefault<UZSharpExportSettings>()->ShouldLootEnumUnderlyingTypeFromTEnumAsByte())
			{
				if (auto numericProperty = CastField<FNumericProperty>(property))
				{
					if (enm = numericProperty->GetIntPropertyEnum(); enm)
					{
						underlyingPropertyClass = numericProperty->GetClass();
					}
				}
			}

			if (enm)
			{
				const FString name = GNameMap[underlyingPropertyClass];
				if (FString* existing = Cache.Find(enm))
				{
					check(*existing == name);
				}
				else
				{
					Cache.Emplace(enm, name);
				}
			}
		}
	}
}

FString ZSharp::FZUEnumUnderlyingTypeLookup::GetEnumUnderlyingTypeName(const UEnum* enm) const
{
	const FString* name = Cache.Find(enm);
	return name ? *name : "int64";
}


