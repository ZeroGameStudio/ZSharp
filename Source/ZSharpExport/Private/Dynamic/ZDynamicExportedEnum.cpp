// Copyright Zero Games. All Rights Reserved.


#include "Dynamic/ZDynamicExportedEnum.h"

#include "ZExportedTypeRegistry.h"

ZSharp::FZDynamicExportedEnum::FZDynamicExportedEnum(UEnum* uenum)
	: Enum(uenum)
{
	check(Enum->IsNative());
	
	bRegistered = FZExportedTypeRegistry::Get().RegisterEnum(this);
}

FString ZSharp::FZDynamicExportedEnum::GetName() const
{
	return Enum->GetName();
}

FString ZSharp::FZDynamicExportedEnum::GetModule() const
{
	FString res;
	const bool bSuc = Enum->GetPackage()->GetName().Split("/", nullptr, &res, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
	check(bSuc);
	return res;
}

ZSharp::EZCallBufferSlotType ZSharp::FZDynamicExportedEnum::GetSlotType() const
{
	return EZCallBufferSlotType::Int64; // @FIXME: UnderlyingType
}

FString ZSharp::FZDynamicExportedEnum::GetUnderlyingType() const
{
	return "int64"; // @FIXME
}

void ZSharp::FZDynamicExportedEnum::ForeachEnumValue(TFunctionRef<void(FString, FString)> action) const
{
	for (int32 i = 0; i < Enum->NumEnums(); ++i)
	{
		const FString Name = Enum->GetNameStringByIndex(i);
		const FString Value = FString::Printf(TEXT("%lld"), Enum->GetValueByIndex(i));
		action(Name, Value);
	}
}


