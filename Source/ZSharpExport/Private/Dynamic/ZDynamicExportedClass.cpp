// Copyright Zero Games. All Rights Reserved.


#include "Dynamic/ZDynamicExportedClass.h"

#include "ZExportedTypeRegistry.h"
#include "ZSharpExportHelpers.h"
#include "Trait/ZConjugateRegistryId.h"

ZSharp::FZDynamicExportedClass::FZDynamicExportedClass(UStruct* ustruct)
	: Struct(ustruct)
	, Flags(EZExportedClassFlags::None)
{
	check(Struct->IsNative());

	if (const auto uclass = Cast<UClass>(ustruct))
	{
		if (uclass->HasAnyClassFlags(CLASS_Interface))
		{
			Flags |= EZExportedClassFlags::Interface;
		}
		else
		{
			Flags |= EZExportedClassFlags::Class;
			// Force UObject to non-abstract because it is used as the fallback concrete type for those non-exported conjugates.
			if (uclass->HasAnyClassFlags(CLASS_Abstract) && uclass != UObject::StaticClass())
			{
				Flags |= EZExportedClassFlags::Abstract;
			}
		}
	}
	else if (const auto ustrct = Cast<UScriptStruct>(ustruct))
	{
		Flags |= EZExportedClassFlags::Struct;
	}
	else
	{
		checkNoEntry();
	}

	bRegistered = FZExportedTypeRegistry::Get().RegisterClass(this);
}

bool ZSharp::FZDynamicExportedClass::IsRegistered() const
{
	return bRegistered;
}

FString ZSharp::FZDynamicExportedClass::GetName() const
{
	return FZSharpExportHelpers::GetUFieldAliasedName(Struct);
}

FString ZSharp::FZDynamicExportedClass::GetModule() const
{
	return FZSharpExportHelpers::GetUFieldModuleName(Struct);
}

FString ZSharp::FZDynamicExportedClass::GetOuterExportName() const
{
	return FZSharpExportHelpers::GetUFieldOuterExportName(Struct);
}

uint16 ZSharp::FZDynamicExportedClass::GetConjugateRegistryId() const
{
	return 0;
}

ZSharp::EZExportedClassFlags ZSharp::FZDynamicExportedClass::GetFlags() const
{
	return Flags;
}

FString ZSharp::FZDynamicExportedClass::GetBaseType() const
{
	if (Struct == UInterface::StaticClass())
	{
		return {};
	}
	
	const UStruct* super = Struct->GetSuperStruct();
	if (!super)
	{
		return {};
	}

	return FZSharpExportHelpers::GetUFieldOuterExportName(super);
}


