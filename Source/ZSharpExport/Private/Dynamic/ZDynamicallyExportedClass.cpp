// Copyright Zero Games. All Rights Reserved.


#include "Dynamic/ZDynamicallyExportedClass.h"

#include "ZExportedTypeRegistry.h"
#include "Dynamic/ZDynamicallyExportedProperty.h"
#include "Reflection/ZReflectionHelper.h"
#include "Static/ZExportHelper.h"

ZSharp::FZDynamicallyExportedClass* ZSharp::FZDynamicallyExportedClass::Create(const UStruct* ustruct)
{
	if (!ustruct->IsNative())
	{
		return nullptr;
	}

	if (!FZReflectionHelper::IsUFieldModuleMapped(ustruct))
	{
		return nullptr;
	}
	
	auto cls = new FZDynamicallyExportedClass { ustruct };
	if (!FZExportedTypeRegistry::Get().RegisterClass(cls))
	{
		delete cls;
		cls = nullptr;
	}

	return cls;
}

FString ZSharp::FZDynamicallyExportedClass::GetName() const
{
	return FZReflectionHelper::GetUFieldAliasedName(Struct);
}

FString ZSharp::FZDynamicallyExportedClass::GetModule() const
{
	return FZReflectionHelper::GetUFieldModuleName(Struct);
}

FString ZSharp::FZDynamicallyExportedClass::GetUnrealFieldPath() const
{
	return Struct->GetPathName();
}

uint16 ZSharp::FZDynamicallyExportedClass::GetConjugateRegistryId() const
{
	return 0;
}

ZSharp::EZExportedClassFlags ZSharp::FZDynamicallyExportedClass::GetFlags() const
{
	return Flags;
}

ZSharp::FZFullyExportedTypeName ZSharp::FZDynamicallyExportedClass::GetBaseType() const
{
	if (Struct == UInterface::StaticClass())
	{
		return {};
	}
	
	const UField* super = FZReflectionHelper::GetUFieldClosestMappedAncestor(Struct->GetSuperStruct());
	if (!super)
	{
		return {};
	}

	return FZExportHelper::GetUFieldFullyExportedName(super);
}

void ZSharp::FZDynamicallyExportedClass::ForeachProperty(TFunctionRef<void(const IZExportedProperty&)> action) const
{
	for (const auto& prop : Properties)
	{
		action(*prop);
	}
}

ZSharp::FZDynamicallyExportedClass::FZDynamicallyExportedClass(const UStruct* ustruct)
	: Struct(ustruct)
	, Flags(EZExportedClassFlags::None)
{
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

	for (TFieldIterator<FProperty> it(Struct, EFieldIteratorFlags::ExcludeSuper); it; ++it)
	{
		FProperty* property = *it;
		for (int32 i = 0; i < property->ArrayDim; ++i)
		{
			FZDynamicallyExportedProperty* exportedProperty = FZDynamicallyExportedProperty::Create(property, i);
			if (!exportedProperty)
			{
				continue;
			}

			Properties.Emplace(exportedProperty);
		}
	}
}


