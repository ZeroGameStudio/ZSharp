// Copyright Zero Games. All Rights Reserved.


#include "Dynamic/ZDynamicExportedEnum.h"

#include "ZExportedTypeRegistry.h"
#include "ZSharpExportHelpers.h"

ZSharp::FZDynamicExportedEnum::FZDynamicExportedEnum(UEnum* uenum)
	: Enum(uenum)
{
	check(Enum->IsNative());
	
	bRegistered = FZExportedTypeRegistry::Get().RegisterEnum(this);
}

bool ZSharp::FZDynamicExportedEnum::IsRegistered() const
{
	return bRegistered;
}

FString ZSharp::FZDynamicExportedEnum::GetName() const
{
	return Enum->GetName();
}

FString ZSharp::FZDynamicExportedEnum::GetModule() const
{
	return FZSharpExportHelpers::GetUFieldModuleName(Enum);
}

ZSharp::EZCallBufferSlotType ZSharp::FZDynamicExportedEnum::GetSlotType() const
{
	return EZCallBufferSlotType::Int64; // @FIXME: UnderlyingType
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


