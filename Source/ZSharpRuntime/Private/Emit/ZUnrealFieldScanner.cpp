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

#define COPY_SIMPLE_PROPERTY(DefName, DtoName) \
			DefName.Type = static_cast<EZPropertyType>(DtoName.Type); \
			DefName.PropertyFlags = static_cast<EPropertyFlags>(DtoName.PropertyFlags); \
			DefName.RepNotifyName = DtoName.RepNotifyName; \
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
			def.Flags = static_cast<EObjectFlags>(dto.Flags);
			def.MetadataMap = MoveTemp(dto.MetadataMap);
			def.SuperPath = dto.SuperPath;
			def.Properties = PropertyDtos2Defs(MoveTemp(dto.Properties));
			def.FunctionFlags = static_cast<EFunctionFlags>(dto.FunctionFlags);
			def.ZCallName = MoveTemp(dto.ZCallName);
			def.RpcId = dto.RpcId;
			def.RpcResponseId = dto.RpcResponseId;
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
				classDef.SuperPath = classDto.SuperPath;
				classDef.Properties = PropertyDtos2Defs(MoveTemp(classDto.Properties));
				classDef.ConfigName = classDto.ConfigName;
				classDef.WithinPath = classDto.WithinPath;
				classDef.ClassFlags = static_cast<EClassFlags>(classDto.ClassFlags);
				classDef.CastFlags = static_cast<EClassCastFlags>(classDto.CastFlags);
				classDef.ImplementedInterfacePaths = MoveTemp(classDto.ImplementedInterfacePaths);
				classDef.Functions = FunctionDtos2Defs(MoveTemp(classDto.Functions));
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
		
		/*
		 * This is for making sure that ScanUnrealFieldsForModule() is executed after ProcessNewlyLoadedUObjects():
		 * 1. if USE_PER_MODULE_UOBJECT_BOOTSTRAP || IS_MONOLITHIC, ProcessNewlyLoadedUObjects() only needs to execute once and has already done before.
		 * 2. if not, ProcessNewlyLoadedUObjects() is executed by FModuleManager::ProcessLoadedObjectsCallback.
		 *    Here ProcessNewlyLoadedUObjects() should have not bound to the delegate
		 *    and fortunately, multicast delegate executes reversed, so we don't need to do nothing else.
		 */
		check(!FModuleManager::Get().OnProcessLoadedObjectsCallback().IsBound());
		ProcessLoadedObjectsDelegate = FModuleManager::Get().OnProcessLoadedObjectsCallback().AddRaw(this, &FZUnrealFieldScanner::ScanUnrealFieldsForModule);
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


