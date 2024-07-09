// Copyright Zero Games. All Rights Reserved.


#include "Dynamic/ZDynamicExportedEnum.h"

#include "ZExportedTypeRegistry.h"
#include "ZSharpExportHelpers.h"

ZSharp::FZDynamicExportedEnum* ZSharp::FZDynamicExportedEnum::Create(UEnum* uenum)
{
	auto enm = new FZDynamicExportedEnum { uenum };
	if (!FZExportedTypeRegistry::Get().RegisterEnum(enm))
	{
		delete enm;
		enm = nullptr;
	}

	return enm;
}

FString ZSharp::FZDynamicExportedEnum::GetName() const
{
	return Enum->GetName();
}

FString ZSharp::FZDynamicExportedEnum::GetModule() const
{
	return FZSharpExportHelpers::GetUFieldModuleName(Enum);
}

FString ZSharp::FZDynamicExportedEnum::GetUnrealFieldPath() const
{
	return Enum->GetPathName();
}

ZSharp::EZCallBufferSlotType ZSharp::FZDynamicExportedEnum::GetSlotType() const
{
	return EZCallBufferSlotType::Int64; // @FIXME: UnderlyingType
}

ZSharp::EZExportedEnumFlags ZSharp::FZDynamicExportedEnum::GetFlags() const
{
	return Flags;
}

FString ZSharp::FZDynamicExportedEnum::GetUnderlyingType() const
{
	return "int64"; // @FIXME
}

void ZSharp::FZDynamicExportedEnum::ForeachEnumValue(TFunctionRef<void(const FString&, const FString&)> action) const
{
	for (int32 i = 0; i < Enum->NumEnums(); ++i)
	{
		const FString Name = Enum->GetNameStringByIndex(i);
		const FString Value = FString::Printf(TEXT("%lld"), Enum->GetValueByIndex(i));
		action(Name, Value);
	}
}

ZSharp::FZDynamicExportedEnum::FZDynamicExportedEnum(UEnum* uenum)
	: Enum(uenum)
	, Flags(EZExportedEnumFlags::None)
{
	check(Enum->IsNative());

	if (Enum->HasAnyEnumFlags(EEnumFlags::Flags))
	{
		Flags |= EZExportedEnumFlags::Flags;
	}
}


