// Copyright Zero Games. All Rights Reserved.

#include "ZDynamicallyExportedEnum.h"

#include "ZExportedTypeRegistry.h"
#include "ZSharpExportSettings.h"
#include "ZExportHelper.h"
#include "ZUEnumUnderlyingTypeLookup.h"

ZSharp::FZDynamicallyExportedEnum* ZSharp::FZDynamicallyExportedEnum::Create(const UEnum* uenum)
{
	if (!FZExportHelper::ShouldExportField(uenum))
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
	return FZExportHelper::GetFieldRedirectedFullName(Enum);
}

FString ZSharp::FZDynamicallyExportedEnum::GetModule() const
{
	return FZExportHelper::GetFieldModuleName(Enum);
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
	return FZUEnumUnderlyingTypeLookup::Get().GetEnumUnderlyingTypeName(Enum);
}

void ZSharp::FZDynamicallyExportedEnum::ForeachEnumValue(TFunctionRef<void(const FString&, const FString&)> action) const
{
	static const FRegexPattern GIdentifierRegex { "^[A-Za-z_][A-Za-z0-9_]*$" };
	
	const auto settings = GetDefault<UZSharpExportSettings>();
	const bool preferScriptName = WITH_METADATA && settings->ShouldUseEnumValueScriptName();
	const bool exportDeprecated = settings->ShouldExportDeprecatedFields();
	const bool exportMax = settings->ShouldExportEnumMaxValue();

	for (int32 i = 0; i < Enum->NumEnums(); ++i)
	{
		FString name;
		// Special cases for some engine enums.
#define nameof(x) StaticEnum<ECollisionChannel>()->GetNameStringByValue(x)
		static const FString GObjectCollisionChannels[] { nameof(ECC_WorldStatic), nameof(ECC_WorldDynamic), nameof(ECC_Pawn), nameof(ECC_PhysicsBody), nameof(ECC_Vehicle), nameof(ECC_Destructible) };
		static const FString GTraceCollisionChannels[] { nameof(ECC_Visibility), nameof(ECC_Camera) };
#undef nameof
		if (Enum == StaticEnum<EObjectTypeQuery>() && i < UE_ARRAY_COUNT(GObjectCollisionChannels))
		{
			name = GObjectCollisionChannels[i];
		}
		else if (Enum == StaticEnum<ETraceTypeQuery>() && i < UE_ARRAY_COUNT(GTraceCollisionChannels))
		{
			name = GTraceCollisionChannels[i];
		}
		else
		{
			name = Enum->GetNameStringByIndex(i);
		}

		const FString scriptName = preferScriptName ? Enum->GetMetaData(TEXT("ScriptName"), i) : "";
		FRegexMatcher matcher { GIdentifierRegex, scriptName };
		if (!scriptName.IsEmpty() && matcher.FindNext())
		{
			name = scriptName;
		}
		
		if (!exportDeprecated && FZExportHelper::IsNameDeprecated(name))
		{
			continue;
		}

		if (!exportMax && name == FString::Printf(TEXT("%s_MAX"), *Enum->GetName()))
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
	if (Enum->HasAnyEnumFlags(EEnumFlags::Flags) || Enum->HasMetaData(TEXT("Bitflags")))
	{
		Flags |= EZExportedEnumFlags::Flags;
	}
}


