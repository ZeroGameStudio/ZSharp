// Copyright Zero Games. All Rights Reserved.

#include "ZDynamicallyExportedEnum.h"

#include "ZExportedTypeRegistry.h"
#include "ZSharpExportSettings.h"
#include "ZExportHelper.h"

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
	return "int64";
}

void ZSharp::FZDynamicallyExportedEnum::ForeachEnumValue(TFunctionRef<void(const FString&, const FString&)> action) const
{
	const auto settings = GetDefault<UZSharpExportSettings>();
	const bool useScriptName = WITH_METADATA && settings->ShouldUseEnumValueScriptName();
	const bool exportDeprecated = settings->ShouldExportDeprecatedFields();

	for (int32 i = 0; i < Enum->NumEnums(); ++i)
	{
		const FString name = useScriptName && Enum->HasMetaData(TEXT("ScriptName"), i) ? Enum->GetMetaData(TEXT("ScriptName"), i) : Enum->GetNameStringByIndex(i);
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
	if (Enum->HasAnyEnumFlags(EEnumFlags::Flags) || Enum->HasMetaData(TEXT("Bitflags")))
	{
		Flags |= EZExportedEnumFlags::Flags;
	}
}


