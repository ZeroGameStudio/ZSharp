// Copyright Zero Games. All Rights Reserved.

#include "ZUnrealFieldScanner.h"

#include "JsonObjectConverter.h"
#include "ZSharpRuntimeLogChannels.h"
#include "Misc/ZStealInvocationList.h"
#include "ZUnrealFieldDefinitionDtos.h"
#include "ALC/IZDefaultAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Emit/ZUnrealFieldDefinitions.h"
#include "Emit/ZUnrealFieldEmitter.h"

namespace ZSharp::ZUnrealFieldScanner_Private
{
	ZSHARP_STEAL_INVOCATION_LIST(GInvocationListMemberPtr)

	static TArray<FZStructDefinition::FPropertyDefault> PropertyDefaultDtos2Defs(TArray<FZStructDefinitionDto_PropertyDefault>&& dtos)
	{
		TArray<FZStructDefinition::FPropertyDefault> defs;
		defs.Reserve(dtos.Num());
		for (auto& dto : dtos)
		{
			FZStructDefinition::FPropertyDefault& propertyDefault = defs.Emplace_GetRef();
			propertyDefault.PropertyChain = MoveTemp(dto.PropertyChain);
			propertyDefault.Buffer = MoveTemp(dto.Buffer);
		}

		return defs;
	}
	
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
			def.RepCondition = static_cast<ELifetimeCondition>(dto.RepCondition);
			def.RepNotifyCondition = static_cast<ELifetimeRepNotifyCondition>(dto.RepNotifyCondition);
			def.IsRepPushBased = dto.IsRepPushBased;

#define COPY_SIMPLE_PROPERTY(DefName, DtoName) \
			DefName.Type = static_cast<EZPropertyType>(DtoName.Type); \
			DefName.EnumUnderlyingType = static_cast<EZEnumUnderlyingType>(DtoName.EnumUnderlyingType); \
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
			def.bIsEventOverride = dto.bIsEventOverride;
			def.Flags = static_cast<EObjectFlags>(dto.Flags);
			def.MetadataMap = MoveTemp(dto.MetadataMap);
			def.TransparentDataMap = MoveTemp(dto.TransparentDataMap);
			def.SuperPath = dto.SuperPath;
			def.Properties = PropertyDtos2Defs(MoveTemp(dto.Properties));
			def.FunctionFlags = static_cast<EFunctionFlags>(dto.FunctionFlags);
			def.ZCallName = MoveTemp(dto.ZCallName);
			def.ValidateZCallName = MoveTemp(dto.ValidateZCallName);
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

		{ // Enums
			for (auto& enumDto : dto.Enums)
			{
				FZEnumDefinition& enumDef = def.Enums.Emplace_GetRef();
				enumDef.Name = enumDto.Name;
				enumDef.Flags = static_cast<EObjectFlags>(enumDto.Flags);
				enumDef.MetadataMap = MoveTemp(enumDto.MetadataMap);
				enumDef.TransparentDataMap = MoveTemp(enumDto.TransparentDataMap);
				enumDef.UnderlyingType = static_cast<EZEnumUnderlyingType>(enumDto.UnderlyingType);
				enumDef.EnumFlags = static_cast<EEnumFlags>(enumDto.EnumFlags);
				for (auto& enumFieldDto : enumDto.Fields)
				{
					FZEnumFieldDefinition& enumFieldDef = enumDef.Fields.Emplace_GetRef();
					enumFieldDef.Name = enumFieldDto.Name;
					enumFieldDef.Flags = static_cast<EObjectFlags>(enumFieldDto.Flags);
					enumFieldDef.MetadataMap = MoveTemp(enumFieldDto.MetadataMap);
					enumFieldDef.TransparentDataMap = MoveTemp(enumFieldDto.TransparentDataMap);
					enumFieldDef.Value = enumFieldDto.Value;
				}
			}
		}

		{ // Structs
			for (auto& scriptStructDto : dto.Structs)
			{
				FZScriptStructDefinition& scriptStructDef = def.Structs.Emplace_GetRef();
				scriptStructDef.Name = scriptStructDto.Name;
				scriptStructDef.Flags = static_cast<EObjectFlags>(scriptStructDto.Flags);
				scriptStructDef.MetadataMap = MoveTemp(scriptStructDto.MetadataMap);
				scriptStructDef.TransparentDataMap = MoveTemp(scriptStructDto.TransparentDataMap);
				scriptStructDef.SuperPath = scriptStructDto.SuperPath;
				scriptStructDef.Properties = PropertyDtos2Defs(MoveTemp(scriptStructDto.Properties));
				scriptStructDef.PropertyDefaults = PropertyDefaultDtos2Defs(MoveTemp(scriptStructDto.PropertyDefaults));
				scriptStructDef.StructFlags = static_cast<EStructFlags>(scriptStructDto.StructFlags);
			}
		}

		{ // Delegates
			for (auto& delegateDto : dto.Delegates)
			{
				FZDelegateDefinition& delegateDef = def.Delegates.Emplace_GetRef();
				delegateDef.Name = delegateDto.Name;
				delegateDef.Flags = static_cast<EObjectFlags>(delegateDto.Flags);
				delegateDef.MetadataMap = MoveTemp(delegateDto.MetadataMap);
				delegateDef.TransparentDataMap = MoveTemp(delegateDto.TransparentDataMap);
				delegateDef.SuperPath = delegateDto.SuperPath;
				delegateDef.Properties = PropertyDtos2Defs(MoveTemp(delegateDto.Properties));
				delegateDef.DelegateType = static_cast<EZDelegateType>(delegateDto.DelegateType);
				delegateDef.OuterClassName = delegateDto.OuterClassName;
			}
		}

		{ // @TODO Interfaces
			
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
				classDef.PropertyDefaults = PropertyDefaultDtos2Defs(MoveTemp(classDto.PropertyDefaults));
				classDef.bConstructor = classDto.bConstructor;
				classDef.bContextual = classDto.bContextual;
				classDef.ConfigName = classDto.ConfigName;
				classDef.WithinPath = classDto.WithinPath;
				classDef.ClassFlags = static_cast<EClassFlags>(classDto.ClassFlags);
				classDef.CastFlags = static_cast<EClassCastFlags>(classDto.CastFlags);
				classDef.ImplementedInterfacePaths = MoveTemp(classDto.ImplementedInterfacePaths);
				classDef.Functions = FunctionDtos2Defs(MoveTemp(classDto.Functions));

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
	static FZUnrealFieldScanner GInstance;

	return GInstance;
}

void ZSharp::FZUnrealFieldScanner::Startup()
{
	// Queue up existing modules.
	TArray<FModuleStatus> moduleStatuses;
	FModuleManager::Get().QueryModules(moduleStatuses);
	for (const auto& status : moduleStatuses)
	{
		if (status.bIsLoaded)
		{
			UE_LOG(LogZSharpEmit, Log, TEXT("[UnrealFieldScanner] Defer scan existing module [%s]."), *status.Name);
			ScanUnrealFieldsForModule(FName(status.Name), false);
		}
	}

	// Queue up post engine init virtual modules.
	PostEngineInitDelegate = FCoreDelegates::OnPostEngineInit.AddLambda([this]
	{
		check(VirtualModuleLookup);
		TArray<const FZEmitVirtualModule*> postEngineInitVirtualModules;
		VirtualModuleLookup->MultiFindPointer(EZEmitVirtualModuleLoadingPhase::PostEngineInit, postEngineInitVirtualModules, true);
		for (const auto& module : postEngineInitVirtualModules)
		{
			ScanUnrealFieldsForModule(FName { module->ModuleName }, true);
		}
	});

	/*
	 * !!!!!!!!!!!!!!!!!!!!! LOW LEVEL HACK !!!!!!!!!!!!!!!!!!!!!
	 * 
	 * BECAUSE MULTICAST DELEGATE INVOKES INNER DELEGATES REVERSELY,
	 * WE HAVE TO USE THIS MAGIC TO ENSURE THAT ScanUnrealFieldsForModule() EXECUTES AFTER ProcessNewlyLoadedUObjects().
	 * 
	 * !!!!!!!!!!!!!!!!!!!!! LOW LEVEL HACK !!!!!!!!!!!!!!!!!!!!!
	 */ 
	auto& delegate = FModuleManager::Get().OnProcessLoadedObjectsCallback();
	ProcessLoadedObjectsDelegate = delegate.AddRaw(this, &FZUnrealFieldScanner::ScanUnrealFieldsForModule);
	auto& invocationList = delegate.*ZUnrealFieldScanner_Private::GInvocationListMemberPtr;
	for (int32 i = invocationList.Num() - 2; i >= 0; --i)
	{
		invocationList.Swap(i, i + 1);
	}
		
	UE_LOG(LogZSharpEmit, Log, TEXT("[UnrealFieldScanner] Successfully hook up ProcessLoadedObjectsDelegate."));
}

void ZSharp::FZUnrealFieldScanner::Shutdown()
{
	FModuleManager::Get().OnProcessLoadedObjectsCallback().Remove(ProcessLoadedObjectsDelegate);
	FCoreDelegates::OnPostEngineInit.Remove(PostEngineInitDelegate);
}

void ZSharp::FZUnrealFieldScanner::ProcessVirtualModules()
{
	if (VirtualModuleLookup)
	{
		return;
	}

	VirtualModuleLookup = TMultiMap<EZEmitVirtualModuleLoadingPhase, FZEmitVirtualModule>{};
	GetDefault<UZSharpRuntimeSettings>()->ForEachEmitVirtualModule([this](const FZEmitVirtualModule& module)
	{
		VirtualModuleLookup->Emplace(module.LoadingPhase, module);
		if (module.LoadingPhase > EZEmitVirtualModuleLoadingPhase::PostEngineInit && ensureAlways(module.ModuleName != module.TargetModule))
		{
			VirtualModuleTargetLookup.Emplace(FName { module.TargetModule }, module);
		}
	});

	TArray<const FZEmitVirtualModule*> earliestPossibleVirtualModules;
	VirtualModuleLookup->MultiFindPointer(EZEmitVirtualModuleLoadingPhase::EarliestPossible, earliestPossibleVirtualModules, true);
	for (const auto& module : earliestPossibleVirtualModules)
	{
		UE_LOG(LogZSharpEmit, Log, TEXT("[UnrealFieldScanner] Defer scan virtual module [%s]."), *module->ModuleName);
		ScanUnrealFieldsForModule(FName { module->ModuleName }, false);
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
	UE_LOG(LogZSharpEmit, Log, TEXT("[UnrealFieldScanner] Prepare to scan module [%s]."), *moduleName.ToString());
	
	if (moduleName.IsNone())
	{
		UE_LOG(LogZSharpEmit, Verbose, TEXT("[UnrealFieldScanner] Skip module without name."));
		return;
	}

	if (ScannedModules.Contains(moduleName))
	{
		UE_LOG(LogZSharpEmit, Verbose, TEXT("[UnrealFieldScanner] Skip scanned module [%s]."), *moduleName.ToString());
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
		
		UE_LOG(LogZSharpEmit, Log, TEXT("[UnrealFieldScanner] Defer scan module [%s] because UObject system has not been ready."), *moduleName.ToString());
		return;
	}

	ProcessVirtualModules(); // Process virtual modules here because we need UZSharpRuntimeSettings CDO.

	TArray<const FZEmitVirtualModule*> relevantVirtualModules;
	VirtualModuleTargetLookup.MultiFindPointer(moduleName, relevantVirtualModules, true);

	for (const auto& module : relevantVirtualModules)
	{
		if (module->LoadingPhase == EZEmitVirtualModuleLoadingPhase::BeforeModule)
		{
			ScanUnrealFieldsForModule(FName { module->ModuleName }, true);
		}
	}

	ON_SCOPE_EXIT
	{
		for (const auto& module : relevantVirtualModules)
		{
			if (module->LoadingPhase == EZEmitVirtualModuleLoadingPhase::AfterModule)
			{
				ScanUnrealFieldsForModule(FName { module->ModuleName }, true);
			}
		}
	};
	
	FlushDeferredModules();

	TArray<FZModuleEmitMetadataSource> sources;
	if (!GetDefault<UZSharpRuntimeSettings>()->GetModuleEmitMetadataSources(moduleName.ToString(), sources))
	{
		UE_LOG(LogZSharpEmit, Verbose, TEXT("[UnrealFieldScanner] Skip unmapped module [%s]."), *moduleName.ToString());
		return;
	}

	UE_LOG(LogZSharpEmit, Log, TEXT("[UnrealFieldScanner] Scan dynamic fields for module [%s]."), *moduleName.ToString());

	for (const auto& source : sources)
	{
		const FString assembly = source.AssemblyName.IsEmpty() ? source.ModuleName : source.AssemblyName;
		const FString moduleNameStr = source.ModuleName;
		FString manifest;
		struct
		{
			const TCHAR* AssemblyName;
			const TCHAR* ModuleName;
			FString* OutManifest;
			uint8 bWithMetadata;
			FString* FatalMessageBuffer;
		} args { *assembly, *moduleNameStr, &manifest, WITH_METADATA };

#if !UE_BUILD_SHIPPING
		FString fatalMessage;
		args.FatalMessageBuffer = &fatalMessage;
		IZSharpClr::Get().GetDefaultAlc().InvokeMethod(ZSHARP_SCANNER_ASSEMBLY_NAME, "ZeroGames.ZSharp.UnrealFieldScanner.UnrealFieldScanner_Interop", "Scan", &args);
		if (!fatalMessage.IsEmpty())
		{
			UE_LOG(LogZSharpEmit, Fatal, TEXT("%s"), *fatalMessage);
		}
#else
		IZSharpClr::Get().GetDefaultAlc().InvokeMethod(ZSHARP_SCANNER_ASSEMBLY_NAME, "ZeroGames.ZSharp.UnrealFieldScanner.UnrealFieldScanner_Interop", "Scan", &args);
#endif

		UE_LOG(LogZSharpEmit, Log, TEXT("[UnrealFieldScanner] Dynamic field manifest for module [%s]: [%s]"), *moduleName.ToString(), *manifest);
	
		if (manifest.IsEmpty())
		{
			return;
		}

		ZUnrealFieldScanner_Private::EmitUnrealFieldsForModule(moduleNameStr, manifest);
	}
}


