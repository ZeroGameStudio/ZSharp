// Copyright Zero Games. All Rights Reserved.


#include "Dynamic/ZDynamicExportedClass.h"

#include "ZExportedTypeRegistry.h"
#include "ZSharpExportHelpers.h"

ZSharp::FZDynamicExportedClass::FZDynamicExportedClass(UStruct* ustruct)
	: Struct(ustruct)
	, Flags(EZExportedClassFlags::None)
{
	check(Struct->IsNative());

	if (auto uclass = Cast<UClass>(ustruct))
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
	else if (auto ustrct = Cast<UScriptStruct>(ustruct))
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

ZSharp::EZExportedClassFlags ZSharp::FZDynamicExportedClass::GetFlags() const
{
	return Flags;
}

FString ZSharp::FZDynamicExportedClass::GetBaseType() const
{
	if (Struct == UInterface::StaticClass())
	{
		return "@NONE";
	}
	
	const UStruct* super = Struct->GetSuperStruct();
	if (!super)
	{
		if (Struct->IsA<UClass>())
		{
			return "@UCLASS";
		}
		else
		{
			return "@USTRUCT";
		}
	}

	return FZSharpExportHelpers::GetUFieldOuterExportName(super);
}

TArray<FString> ZSharp::FZDynamicExportedClass::GetInterfaces() const
{
	checkNoEntry();
	return {};
}

void ZSharp::FZDynamicExportedClass::ForeachMethod(TFunctionRef<void(IZExportedMethod&)> action) const
{
	checkNoEntry();
}


