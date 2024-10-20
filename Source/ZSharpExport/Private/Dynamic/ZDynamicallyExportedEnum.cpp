// Copyright Zero Games. All Rights Reserved.


#include "Dynamic/ZDynamicallyExportedEnum.h"

#include "ZExportedTypeRegistry.h"
#include "ZSharpExportSettings.h"
#include "Reflection/ZReflectionHelper.h"
#include "Static/ZExportHelper.h"

ZSharp::FZDynamicallyExportedEnum* ZSharp::FZDynamicallyExportedEnum::Create(const UEnum* uenum)
{
	if (!uenum->IsNative())
	{
		return nullptr;
	}

	if (!FZReflectionHelper::IsFieldModuleMapped(uenum))
	{
		return nullptr;
	}
	
	auto enm = new FZDynamicallyExportedEnum { uenum };
	if (!FZExportedTypeRegistry::Get().RegisterEnum(enm))
	{
		delete enm;
		enm = nullptr;
	}

	return enm;
}

FString ZSharp::FZDynamicallyExportedEnum::GetName() const
{
	return FZReflectionHelper::GetFieldFullAliasedName(Enum);
}

FString ZSharp::FZDynamicallyExportedEnum::GetModule() const
{
	return FZReflectionHelper::GetFieldModuleName(Enum);
}

FString ZSharp::FZDynamicallyExportedEnum::GetUnrealFieldPath() const
{
	return Enum->GetPathName();
}

ZSharp::EZCallBufferSlotType ZSharp::FZDynamicallyExportedEnum::GetSlotType() const
{
	return EZCallBufferSlotType::Int64;
}

ZSharp::EZExportedEnumFlags ZSharp::FZDynamicallyExportedEnum::GetFlags() const
{
	return Flags;
}

FString ZSharp::FZDynamicallyExportedEnum::GetUnderlyingType() const
{
	return "int64";
}

void ZSharp::FZDynamicallyExportedEnum::ForeachEnumValue(TFunctionRef<void(const FString&, const FString&)> action) const
{
	const bool exportDeprecated = GetDefault<UZSharpExportSettings>()->ShouldExportDeprecatedFields();

	for (int32 i = 0; i < Enum->NumEnums(); ++i)
	{
		const FString name = Enum->GetNameStringByIndex(i);
		if (!exportDeprecated && FZExportHelper::IsNameDeprecated(name))
		{
			continue;
		}
		
		const FString value = FString::Printf(TEXT("%lld"), Enum->GetValueByIndex(i));
		action(name, value);
	}
}

ZSharp::FZDynamicallyExportedEnum::FZDynamicallyExportedEnum(const UEnum* uenum)
	: Enum(uenum)
	, Flags(EZExportedEnumFlags::None)
{
	if (Enum->HasAnyEnumFlags(EEnumFlags::Flags))
	{
		Flags |= EZExportedEnumFlags::Flags;
	}
}


