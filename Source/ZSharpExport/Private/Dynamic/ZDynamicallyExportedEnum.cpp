// Copyright Zero Games. All Rights Reserved.


#include "Dynamic/ZDynamicallyExportedEnum.h"

#include "ZExportedTypeRegistry.h"
#include "ZSharpExportHelpers.h"

ZSharp::FZDynamicallyExportedEnum* ZSharp::FZDynamicallyExportedEnum::Create(UEnum* uenum)
{
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
	return Enum->GetName();
}

FString ZSharp::FZDynamicallyExportedEnum::GetModule() const
{
	return FZSharpExportHelpers::GetUFieldModuleName(Enum);
}

FString ZSharp::FZDynamicallyExportedEnum::GetUnrealFieldPath() const
{
	return Enum->GetPathName();
}

ZSharp::EZCallBufferSlotType ZSharp::FZDynamicallyExportedEnum::GetSlotType() const
{
	return EZCallBufferSlotType::Int64; // @FIXME: UnderlyingType
}

ZSharp::EZExportedEnumFlags ZSharp::FZDynamicallyExportedEnum::GetFlags() const
{
	return Flags;
}

FString ZSharp::FZDynamicallyExportedEnum::GetUnderlyingType() const
{
	return "int64"; // @FIXME
}

void ZSharp::FZDynamicallyExportedEnum::ForeachEnumValue(TFunctionRef<void(const FString&, const FString&)> action) const
{
	for (int32 i = 0; i < Enum->NumEnums(); ++i)
	{
		const FString Name = Enum->GetNameStringByIndex(i);
		const FString Value = FString::Printf(TEXT("%lld"), Enum->GetValueByIndex(i));
		action(Name, Value);
	}
}

ZSharp::FZDynamicallyExportedEnum::FZDynamicallyExportedEnum(UEnum* uenum)
	: Enum(uenum)
	, Flags(EZExportedEnumFlags::None)
{
	check(Enum->IsNative());

	if (Enum->HasAnyEnumFlags(EEnumFlags::Flags))
	{
		Flags |= EZExportedEnumFlags::Flags;
	}
}


