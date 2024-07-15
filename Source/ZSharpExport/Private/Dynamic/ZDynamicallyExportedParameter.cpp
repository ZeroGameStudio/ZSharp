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
	return FZExportHelper::GetFPropertyFullyExportedTypeName(Property);
}

ZSharp::EZExportedParameterFlags ZSharp::FZDynamicallyExportedParameter::GetFlags() const
{
	return Flags;
}

ZSharp::FZDynamicallyExportedParameter::FZDynamicallyExportedParameter(const FProperty* property)
	: Property(property)
	, Flags(EZExportedParameterFlags::In)
{
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
		Flags |= EZExportedParameterFlags::Return;
		Flags &= ~EZExportedParameterFlags::In;
	}
}


