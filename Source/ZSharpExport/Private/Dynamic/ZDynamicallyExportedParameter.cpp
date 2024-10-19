// Copyright Zero Games. All Rights Reserved.


#include "Dynamic/ZDynamicallyExportedParameter.h"

#include "Static/ZExportHelper.h"

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
	return Property->GetName();
}

ZSharp::FZFullyExportedTypeName ZSharp::FZDynamicallyExportedParameter::GetType() const
{
	FZFullyExportedTypeName name = FZExportHelper::GetFPropertyFullyExportedTypeName(Property);
	if (bNullInNotNullOut)
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

bool ZSharp::FZDynamicallyExportedParameter::IsNullInNotNullOut() const
{
	return bNullInNotNullOut;
}

ZSharp::FZDynamicallyExportedParameter::FZDynamicallyExportedParameter(const FProperty* property)
	: Property(property)
	, bNullInNotNullOut(false)
	, Flags(EZExportedParameterFlags::In)
{
	if (!property->HasAnyPropertyFlags(CPF_ConstParm) && property->HasAllPropertyFlags(CPF_OutParm))
	{
		Flags |= EZExportedParameterFlags::Out;
		if (!property->HasAnyPropertyFlags(CPF_ReferenceParm))
		{
			Flags &= ~EZExportedParameterFlags::In;
		}
		else if (FZExportHelper::CanFPropertyBeNullInNotNullOut(property))
		{
			bNullInNotNullOut = true;
		}
	}
	
	if (property->HasAllPropertyFlags(CPF_ReturnParm))
	{
		Flags |= EZExportedParameterFlags::Out;
		Flags |= EZExportedParameterFlags::Return;
		Flags &= ~EZExportedParameterFlags::In;
	}
}


