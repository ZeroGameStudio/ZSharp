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
			Flags |= EZExportedClassFlags::Interface | EZExportedClassFlags::Implementable;
		}
		else
		{
			Flags |= EZExportedClassFlags::Class;

			TSet<const UClass*> exportedInterfaces;
			for (const auto& interface : uclass->Interfaces)
			{
				check(interface.Class->IsNative());
				const UField* mappedInterface = FZReflectionHelper::GetUFieldClosestMappedAncestor(interface.Class);
				if (!mappedInterface)
				{
					continue;
				}

				if (mappedInterface == UInterface::StaticClass() || mappedInterface == UObject::StaticClass())
				{
					continue;
				}

				FZFullyExportedTypeName name = FZExportHelper::GetUFieldFullyExportedName(mappedInterface);
				if (Interfaces.ContainsByPredicate([&name](const FZFullyExportedTypeName& interface){ return interface.Name == name.Name; }))
				{
					continue;
				}

				Interfaces.Emplace(name);

				for (auto currentInterfaceClass = CastChecked<UClass>(mappedInterface); currentInterfaceClass != UInterface::StaticClass(); currentInterfaceClass = currentInterfaceClass->GetSuperClass())
				{
					if (!FZReflectionHelper::IsFieldModuleMapped(currentInterfaceClass))
					{
						continue;
					}
					
					for (const UClass* super = uclass->GetSuperClass(); super; super = super->GetSuperClass())
					{
						if (super->ImplementsInterface(currentInterfaceClass))
						{
							// If some super class also implements this interface,
							// then it should have already exported interface meth.
							exportedInterfaces.Emplace(currentInterfaceClass);
						}
					}

					if (exportedInterfaces.Contains(currentInterfaceClass))
					{
						continue;
					}
					exportedInterfaces.Emplace(currentInterfaceClass);
					
					for (TFieldIterator<UFunction> it(currentInterfaceClass, EFieldIteratorFlags::ExcludeSuper); it; ++it)
					{
						const UFunction* function = *it;

						// If we found a blueprint event,
						// then this is a blueprint implementable interface,
						// which means it doesn't have native UFunction.
						if (function->HasAllFunctionFlags(FUNC_BlueprintEvent))
						{
							break;
						}

						// If implementation class itself has a UFunction with the same name with interface,
						// then only export class version.
						if (uclass->FindFunctionByName(function->GetFName(), EIncludeSuperFlag::ExcludeSuper))
						{
							continue;
						}

						if (!FZExportHelper::ShouldExportFieldBySettings(function))
						{
							continue;
						}

						FZDynamicallyExportedMethod* exportedMethod = FZDynamicallyExportedMethod::Create(function);
						if (!exportedMethod)
						{
							continue;
						}

						Methods.Emplace(exportedMethod);
					}
				}
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
		const UFunction* function = *it;
		if ((Flags & EZExportedClassFlags::Implementable) != EZExportedClassFlags::None && !function->HasAnyFunctionFlags(FUNC_BlueprintEvent))
		{
			Flags &= ~EZExportedClassFlags::Implementable;
		}
		
		if (!FZExportHelper::ShouldExportFieldBySettings(function))
		{
			continue;
		}

		FZDynamicallyExportedMethod* exportedMethod = FZDynamicallyExportedMethod::Create(function);
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


