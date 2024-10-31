// Copyright Zero Games. All Rights Reserved.


#include "ZDynamicallyExportedClass.h"

#include "ZExportedTypeRegistry.h"
#include "ZDynamicallyExportedMethod.h"
#include "ZDynamicallyExportedProperty.h"
#include "Reflection/ZReflectionHelper.h"
#include "ZExportHelper.h"

ZSharp::FZDynamicallyExportedClass* ZSharp::FZDynamicallyExportedClass::Create(const UStruct* ustruct)
{
	if (!ustruct->IsNative())
	{
		return nullptr;
	}

	if (!FZReflectionHelper::IsFieldModuleMapped(ustruct))
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
	return FZReflectionHelper::GetFieldRedirectedFullName(Struct);
}

FString ZSharp::FZDynamicallyExportedClass::GetModule() const
{
	return FZReflectionHelper::GetFieldModuleName(Struct);
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

void ZSharp::FZDynamicallyExportedClass::ForeachInterface(TFunctionRef<void(const FZFullyExportedTypeName&)> action) const
{
	for (const auto& interface : Interfaces)
	{
		action(interface);
	}
}

void ZSharp::FZDynamicallyExportedClass::ForeachMethod(TFunctionRef<void(const IZExportedMethod&)> action) const
{
	for (const auto& method : Methods)
	{
		action(*method);
	}
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

			for (const auto& interface : uclass->Interfaces)
			{
				check(interface.Class->IsNative());
				const UField* mappedInterface = FZReflectionHelper::GetUFieldClosestMappedAncestor(interface.Class);
				if (!mappedInterface)
				{
					continue;
				}

				if (mappedInterface == UInterface::StaticClass())
				{
					continue;
				}

				FZFullyExportedTypeName name = FZExportHelper::GetUFieldFullyExportedName(mappedInterface);
				if (Interfaces.ContainsByPredicate([&name](const FZFullyExportedTypeName& interface){ return interface.Name == name.Name; }))
				{
					continue;
				}

				Interfaces.Emplace(name);
			}
		}
	}
	else if (const auto ustrct = Cast<UScriptStruct>(ustruct))
	{
		Flags |= EZExportedClassFlags::Struct;
		UScriptStruct::ICppStructOps* ops = ustrct->GetCppStructOps();
		if (ops && ops->HasGetTypeHash())
		{
			Flags |= EZExportedClassFlags::Hashable;
		}
	}
	else
	{
		checkNoEntry();
	}

	for (TFieldIterator<UFunction> it(Struct, EFieldIteratorFlags::ExcludeSuper); it; ++it)
	{
		if (!FZExportHelper::ShouldExportFieldBySettings(*it))
		{
			continue;
		}

		FZDynamicallyExportedMethod* exportedMethod = FZDynamicallyExportedMethod::Create(*it);
		if (!exportedMethod)
		{
			continue;
		}

		Methods.Emplace(exportedMethod);
	}

	for (TFieldIterator<FProperty> it(Struct, EFieldIteratorFlags::ExcludeSuper); it; ++it)
	{
		FProperty* property = *it;
		if (!FZExportHelper::ShouldExportFieldBySettings(property))
		{
			continue;
		}
		
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


