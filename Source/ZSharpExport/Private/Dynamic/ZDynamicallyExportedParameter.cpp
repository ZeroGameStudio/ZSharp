// Copyright Zero Games. All Rights Reserved.

#include "ZDynamicallyExportedParameter.h"

#include "ZExportHelper.h"

ZSharp::FZDynamicallyExportedParameter* ZSharp::FZDynamicallyExportedParameter::Create(const FProperty* property)
{
	auto param = new FZDynamicallyExportedParameter { property };
	if (!param->IsValid())
	{
		delete param;
		param = nullptr;
	}

	return param;
}

FString ZSharp::FZDynamicallyExportedParameter::GetName() const
{
	return FZExportHelper::GetFieldRedirectedName(Property);
}

ZSharp::FZFullyExportedTypeName ZSharp::FZDynamicallyExportedParameter::GetType() const
{
	FZFullyExportedTypeName name = FZExportHelper::GetFieldFullyExportedTypeName(Property);
	if (bNullInNotNullOut && (Flags & EZExportedParameterFlags::In) != EZExportedParameterFlags::None)
	{
		name.bNullable = true;
	}

	return name;
}

FString ZSharp::FZDynamicallyExportedParameter::GetUnderlyingType() const
{
	return FZExportHelper::GetUEnumFromProperty(Property) ? "int64" : FString{};
}

ZSharp::EZExportedParameterFlags ZSharp::FZDynamicallyExportedParameter::GetFlags() const
{
	return Flags;
}

ZSharp::FZExportedDefaultValue ZSharp::FZDynamicallyExportedParameter::GetDefaultValue() const
{
	return FZExportHelper::GetParameterDefaultValue(Property);
}

bool ZSharp::FZDynamicallyExportedParameter::IsNullInNotNullOut() const
{
	return bNullInNotNullOut;
}

ZSharp::FZDynamicallyExportedParameter::FZDynamicallyExportedParameter(const FProperty* property)
	: Property(property)
	, bNullInNotNullOut(false)
	, Flags(EZExportedParameterFlags::In)
{
	if (FZExportHelper::CanFPropertyBeNullInNotNullOut(property))
	{
		bNullInNotNullOut = true;
	}
	
	if (!property->HasAnyPropertyFlags(CPF_ConstParm) && property->HasAllPropertyFlags(CPF_OutParm))
	{
		Flags |= EZExportedParameterFlags::Out;
		if (!property->HasAnyPropertyFlags(CPF_ReferenceParm))
		{
			Flags &= ~EZExportedParameterFlags::In;
		}
	}
	
	if (property->HasAllPropertyFlags(CPF_ReturnParm))
	{
		Flags |= EZExportedParameterFlags::Out;
		Flags |= EZExportedParameterFlags::Return;
		Flags &= ~EZExportedParameterFlags::In;
	}
}


