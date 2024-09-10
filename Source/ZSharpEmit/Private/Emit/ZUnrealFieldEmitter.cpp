// Copyright Zero Games. All Rights Reserved.


#include "ZUnrealFieldEmitter.h"

#include "ZSharpEmitLogChannels.h"
#include "Algo/TopologicalSort.h"
#include "Emit/Field/ZSharpClass.h"
#include "Field/ZUnrealFieldDefinitions.h"

ZSharp::FZUnrealFieldEmitter& ZSharp::FZUnrealFieldEmitter::Get()
{
	static FZUnrealFieldEmitter GSingleton;

	return GSingleton;
}

UPackage* ZSharp::FZUnrealFieldEmitter::Emit(FZPackageDefinition& def)
{
	UE_CLOG(bEmitting, LogZSharpEmit, Fatal, TEXT("Emit is a package-wide atomic operation so can't invoke recursively!!!"));
	TGuardValue recursionGuard { bEmitting, true };
	
	EmitPackage(def);
	return def.Package;
}

void ZSharp::FZUnrealFieldEmitter::EmitPackage(FZPackageDefinition& def) const
{
	UE_CLOG(!!FindPackage(nullptr, *def.Path.ToString()), LogZSharpEmit, Fatal, TEXT("Package [%s] already exists!!!"), *def.Path.ToString());
	UPackage* pak = CreatePackage(*def.Path.ToString());
	def.Package = pak;

	// Engine code only marks script packages as compiled-in but not root.
	// This is because compiled-in packages are always referenced by compiled-in fields which is mark as root.
	// So compiled-in packages never get GCed and we keep sync with engine code.
	pak->SetPackageFlags(PKG_CompiledIn);

	// Enums have no dependency so we can emit them just one step.
	for (auto& pair : def.EnumMap)
	{
		EmitEnum(pak, pair.Value);
	}
	
	// Other fields have dependency so we emit skeleton (placeholder) first.
	for (auto& pair : def.StructMap)
	{
		EmitStructSkeleton(pak, pair.Value);
	}

	for (auto& pair : def.ClassMap)
	{
		EmitClassSkeleton(pak, pair.Value);
	}

	for (auto& pair : def.InterfaceMap)
	{
		EmitInterfaceSkeleton(pak, pair.Value);
	}

	for (auto& pair : def.DelegateMap)
	{
		EmitDelegateSkeleton(pak, pair.Value);
	}

	// Now that all fields are in memory (but not fully initialized) and we can setup dependency.
	for (const auto& pair : def.StructMap)
	{
		FinishEmitStruct(pak, pair.Value);
	}

	// Subclass may depend on super/within class's data i.e. ClassConfigName, SparseClassDataStruct.
	// So we sort classes to ensure super/within class get constructed before subclass.
	// Algo::TopologicalSort doesn't support projection or custom hash, so we manually make a projection.
	{
		TArray<const UZSharpClass*> classes;
		TMap<const UZSharpClass*, const FZClassDefinition*> map;
		for (const auto& pair : def.ClassMap)
		{
			classes.Emplace(pair.Value.Class);
			map.Emplace(pair.Value.Class, &pair.Value);
		}
		
		Algo::TopologicalSort(classes, [&map](const UZSharpClass* cls)
		{
			const FZClassDefinition* classDef = map.FindChecked(cls);
			TArray<UZSharpClass*, TInlineAllocator<2>> dependencies;
			
			if (const auto superClass = FindObjectChecked<UClass>(nullptr, *classDef->SuperPath.ToString()); superClass->UObject::IsA<UZSharpClass>())
			{
				dependencies.Emplace(static_cast<UZSharpClass*>(superClass));
			}
			
			if (!classDef->WithinPath.IsNone())
			{
				if (const auto withinClass = FindObjectChecked<UClass>(nullptr, *classDef->WithinPath.ToString()); withinClass->UObject::IsA<UZSharpClass>())
				{
					dependencies.Emplace(static_cast<UZSharpClass*>(withinClass));
				}
			}
			
			return dependencies;
		});
		
		for (const auto cls : classes)
		{
			const FZClassDefinition* classDef = map.FindChecked(cls);
			FinishEmitClass(pak, *classDef);
		}
	}

	for (const auto& pair : def.InterfaceMap)
	{
		FinishEmitInterface(pak, pair.Value);
	}

	for (const auto& pair : def.DelegateMap)
	{
		FinishEmitDelegate(pak, pair.Value);
	}

	// Finally, create CDO for all classes.
	for (const auto& pair : def.ClassMap)
	{
		(void)pair.Value.Class->GetDefaultObject();
	}
}

void ZSharp::FZUnrealFieldEmitter::EmitEnum(UPackage* pak, FZEnumDefinition& def) const
{
	// @TODO
}

void ZSharp::FZUnrealFieldEmitter::EmitStructSkeleton(UPackage* pak, FZScriptStructDefinition& def) const
{
	// @TODO
}

void ZSharp::FZUnrealFieldEmitter::EmitClassSkeleton(UPackage* pak, FZClassDefinition& def) const
{
	// Migrate from GetPrivateStaticClassBody().
	static EObjectFlags GCompiledInFlags = RF_Public | RF_Standalone | RF_Transient | RF_MarkAsNative | RF_MarkAsRootSet;
	// Migrate from static constructor of UClass.
	static EClassFlags GCompiledInClassFlags = CLASS_Native;
	
	FStaticConstructObjectParameters params { UZSharpClass::StaticClass() };
	params.Outer = pak;
	params.Name = *def.Name.ToString();
	params.SetFlags = def.Flags | GCompiledInFlags;
	
	const auto cls = static_cast<UZSharpClass*>(StaticConstructObject_Internal(params));
	def.Class = cls;

	// Migrate from UClass(EStaticConstructor).
	// UZSharpClass::ClassConstructor is a compile-time constant and set by UZSharpClass itself so we don't care it here.
	// UZSharpClass has no ClassVTableHelperCtorCaller and CppClassStaticFunctions so will directly copy from super in Bind().
	cls->ClassUnique = 0;
	cls->bCooked = false;
	cls->bLayoutChanging = false;
	cls->ClassFlags = def.ClassFlags | GCompiledInClassFlags;
	cls->ClassCastFlags = def.CastFlags;
#if WITH_EDITORONLY_DATA
	cls->ClassGeneratedBy = nullptr; // UZSharpClass doesn't depend on blueprint.
	cls->PropertiesPendingDestruction = nullptr;
#endif
	// @TODO: UZSharpClass is not support NetFields yet.
	cls->ClassDefaultObject = nullptr;
	cls->SetSparseClassDataStruct(nullptr);
	
	// Migrate from UECodeGen_Private::ConstructUClass().
	FCppClassTypeInfoStatic typeInfo;
	typeInfo.bIsAbstract = cls->HasAllClassFlags(CLASS_Abstract);
	cls->SetCppTypeInfoStatic(&typeInfo);
	cls->ClassConfigName = def.ConfigName;
}

void ZSharp::FZUnrealFieldEmitter::EmitInterfaceSkeleton(UPackage* pak, FZInterfaceDefinition& def) const
{
	// @TODO
}

void ZSharp::FZUnrealFieldEmitter::EmitDelegateSkeleton(UPackage* pak, FZDelegateDefinition& def) const
{
	// @TODO
}

void ZSharp::FZUnrealFieldEmitter::FinishEmitStruct(UPackage* pak, const FZScriptStructDefinition& def) const
{
	// @TODO
}

void ZSharp::FZUnrealFieldEmitter::FinishEmitClass(UPackage* pak, const FZClassDefinition& def) const
{
	UZSharpClass* cls = def.Class;

	// Migrate from InitializePrivateStaticClass().
	// UZSharpClass must have super class.
	check(!def.SuperPath.IsNone());
	const auto superClass = FindObjectChecked<UClass>(nullptr, *def.SuperPath.ToString());
	cls->SetSuperStruct(superClass);
	cls->ClassFlags |= superClass->ClassFlags & CLASS_Inherit;

	if (!def.WithinPath.IsNone())
	{
		const auto withinClass = FindObjectChecked<UClass>(nullptr, *def.WithinPath.ToString());
		cls->ClassWithin = withinClass;
	}

	cls->RegisterDependencies();

	// UECodeGen_Private::ConstructUClass() calls UObjectForceRegistration() to initialize statically constructed class.
	// This process sets value for ClassPrivate, OuterPrivate, etc. and add add the class to object hash.
	// StaticConstructObject_Internal() has already done these for us so we don't need to do anymore.

	cls->ClassFlags |= CLASS_Constructed;

	// Actually UZSharpClass can never been intrinsic but we want to keep sync with engine code.
	if (!cls->HasAnyClassFlags(CLASS_Intrinsic))
	{
		check(!cls->HasAnyClassFlags(CLASS_TokenStreamAssembled));
		cls->ReferenceSchema.Reset();
	}

	// @TODO: Emit class functions.
	// @TODO: Emit class properties.

	// We have already set CppTypeInfo before so we don't need to do here.
	// But it is still necessary to match ClassConfigName with super.
	while (UClass* super = cls->GetSuperClass())
	{
		if (!super->ClassConfigName.IsNone())
		{
			if (cls->ClassConfigName.IsNone())
			{
				cls->ClassConfigName = super->ClassConfigName;
			}
			else
			{
				UE_CLOG(cls->ClassConfigName != super->ClassConfigName, LogZSharpEmit, Fatal, TEXT("Class [%s] has different ClassConfigName [%s] with super [%s].[%s]!!!"),
					*GetNameSafe(cls),
					*cls->ClassConfigName.ToString(),
					*GetNameSafe(super),
					*super->ClassConfigName.ToString());
			}
			break;
		}
	}

	if (!def.ImplementedInterfacePaths.IsEmpty())
	{
		cls->Interfaces.Reserve(def.ImplementedInterfacePaths.Num());
		for (const auto& implementedInterfacePath : def.ImplementedInterfacePaths)
		{
			UClass* interfaceClass = FindObjectChecked<UClass>(nullptr, *implementedInterfacePath.ToString());
			check(interfaceClass->HasAllClassFlags(CLASS_Interface));
			// Interfaces implemented by UZSharpClass is always regarded as implemented in blueprint.
			FImplementedInterface implementedInterface { interfaceClass, 0, true };
			cls->Interfaces.Emplace(implementedInterface);
		}
	}

	AddMetadata(cls, def.MetadataMap);

	cls->Bind();
	cls->StaticLink(true);
	cls->AssembleReferenceTokenStream();
	cls->SetSparseClassDataStruct(cls->GetSparseClassDataArchetypeStruct());
}

void ZSharp::FZUnrealFieldEmitter::FinishEmitInterface(UPackage* pak, const FZInterfaceDefinition& def) const
{
	// @TODO
}

void ZSharp::FZUnrealFieldEmitter::FinishEmitDelegate(UPackage* pak, const FZDelegateDefinition& def) const
{
	// @TODO
}

void ZSharp::FZUnrealFieldEmitter::AddMetadata(UObject* object, const TMap<FName, FString>& metadata) const
{
#if WITH_METADATA
	if (!metadata.IsEmpty())
	{
		UMetaData* target = object->GetOutermost()->GetMetaData();
		for (const auto& pair : metadata)
		{
			target->SetValue(object, pair.Key, *pair.Value);
		}
	}
#endif
}


