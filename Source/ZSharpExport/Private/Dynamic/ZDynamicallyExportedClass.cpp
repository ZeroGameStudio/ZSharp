// Copyright Zero Games. All Rights Reserved.

#include "ZDynamicallyExportedClass.h"

#include "ZExportedTypeRegistry.h"
#include "ZDynamicallyExportedMethod.h"
#include "ZDynamicallyExportedProperty.h"
#include "ZExportHelper.h"
#include "Reflection/ZReflectionHelper.h"

ZSharp::FZDynamicallyExportedClass* ZSharp::FZDynamicallyExportedClass::Create(const UStruct* ustruct)
{
	if (!FZExportHelper::ShouldExportField(ustruct))
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
	return FZExportHelper::GetFieldRedirectedFullName(Struct);
}

FString ZSharp::FZDynamicallyExportedClass::GetModule() const
{
	return FZExportHelper::GetFieldModuleName(Struct);
}

FString ZSharp::FZDynamicallyExportedClass::GetUnrealFieldPath() const
{
	return Struct->GetPathName();
}

uint16 ZSharp::FZDynamicallyExportedClass::GetConjugateRegistryId() const
{
	return 0;
}

FString ZSharp::FZDynamicallyExportedClass::GetConjugateKey() const
{
	return FZReflectionHelper::GetFieldConjugateKey(Struct);
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
	
	const UField* super = FZExportHelper::GetUFieldClosestExportedAncestor(Struct->GetSuperStruct());
	if (!super)
	{
		return {};
	}

	return FZExportHelper::GetFieldFullyExportedTypeName(super);
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
	if (auto uclass = Cast<const UClass>(ustruct))
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
				const UField* exportedInterface = FZExportHelper::GetUFieldClosestExportedAncestor(interface.Class);
				if (!exportedInterface)
				{
					continue;
				}

				if (exportedInterface == UInterface::StaticClass() || exportedInterface == UObject::StaticClass())
				{
					continue;
				}

				FZFullyExportedTypeName name = FZExportHelper::GetFieldFullyExportedTypeName(exportedInterface);
				if (Interfaces.ContainsByPredicate([&name](const FZFullyExportedTypeName& interface){ return interface.Name == name.Name; }))
				{
					continue;
				}

				Interfaces.Emplace(name);

				for (auto currentInterfaceClass = CastChecked<UClass>(exportedInterface); currentInterfaceClass != UInterface::StaticClass(); currentInterfaceClass = currentInterfaceClass->GetSuperClass())
				{
					// Early out if this interface is not exported.
					if (!FZExportHelper::ShouldExportField(currentInterfaceClass))
					{
						continue;
					}
					
					for (const UClass* super = uclass->GetSuperClass(); super; super = super->GetSuperClass())
					{
						if (super->ImplementsInterface(currentInterfaceClass))
						{
							// If some super class also implements this interface,
							// then it should have already exported interface methods.
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
	else if (auto ustrct = Cast<const UScriptStruct>(ustruct))
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


