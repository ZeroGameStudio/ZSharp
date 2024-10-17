// Copyright Zero Games. All Rights Reserved.


#include "ZUnrealFieldScanner.h"

#include "JsonObjectConverter.h"
#include "ZSharpRuntimeLogChannels.h"
#include "ZSharpRuntimeSettings.h"
#include "ZUnrealFieldDefinitionDtos.h"
#include "ALC/IZSlimAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Emit/ZUnrealFieldDefinitions.h"
#include "Emit/ZUnrealFieldEmitter.h"

namespace ZSharp::ZUnrealFieldScanner_Private
{
	// This magic helps us to access multicast delegate invocation list.
	static TArray<TDelegateBase<FNotThreadSafeNotCheckedDelegateMode>, FMulticastInvocationListAllocatorType>(TMulticastDelegateBase<FDefaultDelegateUserPolicy>::*GInvocationListMemberPtr);
	template <decltype(GInvocationListMemberPtr) MemberPtr>
	struct TInvocationListMemberPtrInitializer
	{
		TInvocationListMemberPtrInitializer()
		{
			GInvocationListMemberPtr = MemberPtr;
		}
		static TInvocationListMemberPtrInitializer GInstance;
	};
	template <decltype(GInvocationListMemberPtr) MemberPtr> TInvocationListMemberPtrInitializer<MemberPtr> TInvocationListMemberPtrInitializer<MemberPtr>::GInstance;
	template struct TInvocationListMemberPtrInitializer <&TMulticastDelegateBase<FDefaultDelegateUserPolicy>::InvocationList>;

	static TArray<FZPropertyDefinition> PropertyDtos2Defs(TArray<FZPropertyDefinitionDto>&& dtos)
	{
		TArray<FZPropertyDefinition> defs;
		defs.Reserve(dtos.Num());
		
		for (auto& dto : dtos)
		{
			FZPropertyDefinition& def = defs.Emplace_GetRef();
			def.Name = dto.Name;
			def.Flags = static_cast<EObjectFlags>(dto.Flags);
			def.MetadataMap = MoveTemp(dto.MetadataMap);
			def.TransparentDataMap = MoveTemp(dto.TransparentDataMap);
			def.RepNotifyName = dto.RepNotifyName;

#define COPY_SIMPLE_PROPERTY(DefName, DtoName) \
			DefName.Type = static_cast<EZPropertyType>(DtoName.Type); \
			DefName.PropertyFlags = static_cast<EPropertyFlags>(DtoName.PropertyFlags); \
			DefName.DescriptorFieldPath = DtoName.DescriptorFieldPath;

			COPY_SIMPLE_PROPERTY(def, dto);
			COPY_SIMPLE_PROPERTY(def.InnerProperty, dto.InnerProperty);
			COPY_SIMPLE_PROPERTY(def.OuterProperty, dto.OuterProperty);

#undef COPY_SIMPLE_PROPERTY
		}

		return defs;
	}

	static TArray<FZFunctionDefinition> FunctionDtos2Defs(TArray<FZFunctionDefinitionDto>&& dtoMap)
	{
		TArray<FZFunctionDefinition> defs;
		defs.Reserve(dtoMap.Num());
		
		for (auto& dto : dtoMap)
		{
			FZFunctionDefinition& def = defs.Emplace_GetRef();
			def.Name = dto.Name;
			def.IsEventOverride = dto.IsEventOverride;
			def.Flags = static_cast<EObjectFlags>(dto.Flags);
			def.MetadataMap = MoveTemp(dto.MetadataMap);
			def.TransparentDataMap = MoveTemp(dto.TransparentDataMap);
			def.SuperPath = dto.SuperPath;
			def.Properties = PropertyDtos2Defs(MoveTemp(dto.Properties));
			def.FunctionFlags = static_cast<EFunctionFlags>(dto.FunctionFlags);
			def.ZCallName = MoveTemp(dto.ZCallName);
			def.RpcId = dto.RpcId;
			def.RpcResponseId = dto.RpcResponseId;
			def.CustomThunkName = dto.CustomThunkName;
		}

		return defs;
	}
	
	static FZUnrealFieldManifest ManifestDto2Def(FZUnrealFieldManifestDto&& dto)
	{
		FZUnrealFieldManifest def;
		def.ModuleName = dto.ModuleName;

		{ // @TODO Enums
			
		}

		{ // @TODO Structs
			
		}

		{ // Classes
			for (auto& classDto : dto.Classes)
			{
				FZClassDefinition& classDef = def.Classes.Emplace_GetRef();
				classDef.Name = classDto.Name;
				classDef.Flags = static_cast<EObjectFlags>(classDto.Flags);
				classDef.MetadataMap = MoveTemp(classDto.MetadataMap);
				classDef.TransparentDataMap = MoveTemp(classDto.TransparentDataMap);
				classDef.SuperPath = classDto.SuperPath;
				classDef.Properties = PropertyDtos2Defs(MoveTemp(classDto.Properties));
				classDef.ConfigName = classDto.ConfigName;
				classDef.WithinPath = classDto.WithinPath;
				classDef.ClassFlags = static_cast<EClassFlags>(classDto.ClassFlags);
				classDef.CastFlags = static_cast<EClassCastFlags>(classDto.CastFlags);
				classDef.ImplementedInterfacePaths = MoveTemp(classDto.ImplementedInterfacePaths);
				classDef.Functions = FunctionDtos2Defs(MoveTemp(classDto.Functions));

				classDef.PropertyDefaults.Reserve(classDto.PropertyDefaults.Num());
				for (auto& propertyDefaultDto : classDto.PropertyDefaults)
				{
					FZClassDefinition::FPropertyDefault& propertyDefault = classDef.PropertyDefaults.Emplace_GetRef();
					propertyDefault.PropertyChain = MoveTemp(propertyDefaultDto.PropertyChain);
					propertyDefault.Buffer = MoveTemp(propertyDefaultDto.Buffer);
				}

				classDef.DefaultSubobjects.Reserve(classDto.DefaultSubobjects.Num());
				for (auto& defaultSubobjectDto : classDto.DefaultSubobjects)
				{
					FZClassDefinition::FDefaultSubobject& defaultSubobject = classDef.DefaultSubobjects.Emplace_GetRef();
					defaultSubobject.Name = defaultSubobjectDto.Name;
					defaultSubobject.ClassPath = defaultSubobjectDto.ClassPath;
					defaultSubobject.bOptional = defaultSubobjectDto.bOptional;
					defaultSubobject.bTransient = defaultSubobjectDto.bTransient;
					defaultSubobject.PropertyName = defaultSubobjectDto.PropertyName;
					defaultSubobject.bRootComponent = defaultSubobjectDto.bRootComponent;
					defaultSubobject.AttachParentDefaultSubobjectName = defaultSubobjectDto.AttachParentDefaultSubobjectName;
					defaultSubobject.AttachSocketName = defaultSubobjectDto.AttachSocketName;
				}

				classDef.DefaultSubobjectOverrides.Reserve(classDto.DefaultSubobjectOverrides.Num());
				for (auto& defaultObjectOverrideDto : classDto.DefaultSubobjectOverrides)
				{
					FZClassDefinition::FDefaultSubobjectOverride& defaultObjectOverride = classDef.DefaultSubobjectOverrides.Emplace_GetRef();
					defaultObjectOverride.Name = defaultObjectOverrideDto.Name;
					defaultObjectOverride.ClassPath = defaultObjectOverrideDto.ClassPath;
				}

				classDef.FieldNotifies = MoveTemp(classDto.FieldNotifies);
			}
		}

		{ // @TODO Interfaces
			
		}

		{ // @TODO Delegates
			
		}

		return def;
	}
	
	static void EmitUnrealFieldsForModule(const FString& moduleName, const FString& manifestJson)
	{
		FZUnrealFieldManifestDto dto;
		if (!FJsonObjectConverter::JsonObjectStringToUStruct<FZUnrealFieldManifestDto>(manifestJson, &dto))
		{
			UE_LOG(LogZSharpEmit, Fatal, TEXT("Fail to emit unreal field for module [%s]!!! Manifest: [%s]"), *moduleName, *manifestJson);
			return;
		}

		FZUnrealFieldManifest manifest = ManifestDto2Def(MoveTemp(dto));
		FZUnrealFieldEmitter::Get().Emit(manifest);
	}
}

ZSharp::FZUnrealFieldScanner& ZSharp::FZUnrealFieldScanner::Get()
{
	static FZUnrealFieldScanner GSingleton;

	return GSingleton;
}

void ZSharp::FZUnrealFieldScanner::Startup()
{
	ScannerAlc = IZSharpClr::Get().CreateSlimAlc("__UnrealFieldScanner");
	if (ScannerAlc->LoadAssembly(ZSHARP_SCANNER_ASSEMBLY_NAME) == EZLoadAssemblyErrorCode::Succeed)
	{
		TArray<FModuleStatus> moduleStatuses;
		FModuleManager::Get().QueryModules(moduleStatuses);
		for (const auto& status : moduleStatuses)
		{
			if (status.bIsLoaded)
			{
				ScanUnrealFieldsForModule(FName(status.Name), false);
			}
		}

		// Since 5.5, ProcessNewlyLoadedUObjects() binds to FModuleManager::ProcessLoadedObjectsCallback super early.
		// And because multicast delegate invokes inner delegates reversely,
		// we have to use this magic to ensure that ScanUnrealFieldsForModule() executes after ProcessNewlyLoadedUObjects().
		auto& delegate = FModuleManager::Get().OnProcessLoadedObjectsCallback();
		ProcessLoadedObjectsDelegate = delegate.AddRaw(this, &FZUnrealFieldScanner::ScanUnrealFieldsForModule);
		auto& invocationList = delegate.*ZUnrealFieldScanner_Private::GInvocationListMemberPtr;
		check(invocationList.Num() == 2);
		invocationList.Swap(0, 1);
	}
	else
	{
		ScannerAlc->Unload();
		ScannerAlc = nullptr;
	}
}

void ZSharp::FZUnrealFieldScanner::Shutdown()
{
	if (ScannerAlc)
	{
		FModuleManager::Get().OnProcessLoadedObjectsCallback().Remove(ProcessLoadedObjectsDelegate);
		ScannerAlc->Unload();
		ScannerAlc = nullptr;
	}
}

void ZSharp::FZUnrealFieldScanner::FlushDeferredModules()
{
	if (!DeferredModules)
	{
		return;
	}

	TArray<FName> deferredModules = MoveTemp(*DeferredModules);
	DeferredModules.Reset();
	
	for (const auto& deferredModuleName : deferredModules)
	{
		ScanUnrealFieldsForModule(deferredModuleName, true);
	}
}

void ZSharp::FZUnrealFieldScanner::ScanUnrealFieldsForModule(FName moduleName, bool canProcessNewlyLoadedObject)
{
	if (moduleName.IsNone())
	{
		return;
	}

	if (ScannedModules.Contains(moduleName))
	{
		return;
	}
	ScannedModules.Emplace(moduleName);
	
	if (!canProcessNewlyLoadedObject)
	{
		if (!DeferredModules)
		{
			DeferredModules = TArray<FName>{};
		}
		DeferredModules->AddUnique(moduleName);
		return;
	}

	FlushDeferredModules();

	const FZModuleMappingContext* ctx = GetDefault<UZSharpRuntimeSettings>()->GetModuleMappingContext(moduleName.ToString());
	if (!ctx)
	{
		return;
	}

	if (!ctx->bHasDynamicFields)
	{
		return;
	}
	
	const FString assembly = ctx->AssemblyName;
	const FString moduleNameStr = moduleName.ToString();
	FString outManifest;
	struct
	{
		const TCHAR* AssemblyName;
		const TCHAR* ModuleName;
		FString* OutManifest;
		uint8 bWithMetadata;
	} args { *assembly, *moduleNameStr, &outManifest, WITH_METADATA };
	ScannerAlc->InvokeMethod(ZSHARP_SCANNER_ASSEMBLY_NAME, "ZeroGames.ZSharp.UnrealFieldScanner.UnrealFieldScanner_Interop", "Scan", &args);

	if (outManifest.IsEmpty())
	{
		return;
	}

	ZUnrealFieldScanner_Private::EmitUnrealFieldsForModule(moduleNameStr, outManifest);
}


