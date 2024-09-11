// Copyright Zero Games. All Rights Reserved.


#include "ZUnrealFieldScanner.h"

#include "JsonObjectConverter.h"
#include "ZSharpEmitLogChannels.h"
#include "ZSharpEmitSettings.h"
#include "ZUnrealFieldDefinitionDtos.h"
#include "ALC/IZSlimAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Emit/ZUnrealFieldDefinitions.h"
#include "Emit/ZUnrealFieldEmitter.h"

namespace ZSharp::ZUnrealFieldScanner_Private
{
	static TMap<FName, FZPropertyDefinition> PropertyDtoMap2DefMap(TMap<FName, FZPropertyDefinitionDto>&& dtoMap)
	{
		TMap<FName, FZPropertyDefinition> defMap;
		defMap.Reserve(dtoMap.Num());
		
		for (auto& pair : dtoMap)
		{
			FZPropertyDefinitionDto& dto = pair.Value;
			FZPropertyDefinition& def = defMap.Emplace(dto.Name);
			def.Name = dto.Name;
			def.Flags = static_cast<EObjectFlags>(dto.Flags);
			def.MetadataMap = MoveTemp(dto.MetadataMap);
		}

		return defMap;
	}

	static TMap<FName, FZFunctionDefinition> FunctionDtoMap2DefMap(TMap<FName, FZFunctionDefinitionDto>&& dtoMap)
	{
		TMap<FName, FZFunctionDefinition> defMap;
		defMap.Reserve(dtoMap.Num());
		
		for (auto& pair : dtoMap)
		{
			FZFunctionDefinitionDto& dto = pair.Value;
			FZFunctionDefinition& def = defMap.Emplace(dto.Name);
			def.Name = dto.Name;
			def.Flags = static_cast<EObjectFlags>(dto.Flags);
			def.MetadataMap = MoveTemp(dto.MetadataMap);
			def.SuperPath = dto.SuperPath;
			def.PropertyMap = PropertyDtoMap2DefMap(MoveTemp(dto.PropertyMap));
			def.FunctionFlags = static_cast<EFunctionFlags>(dto.FunctionFlags);
			def.RpcId = dto.RpcId;
			def.RpcResponseId = dto.RpcResponseId;
		}

		return defMap;
	}
	
	static FZPackageDefinition PackageDto2Def(FZPackageDefinitionDto&& dto)
	{
		FZPackageDefinition def;
		def.Path = dto.Path;

		{ // @TODO Enums
			
		}

		{ // @TODO Structs
			
		}

		{ // Classes
			for (auto& pair : dto.ClassMap)
			{
				FZClassDefinitionDto& classDto = pair.Value;
				FZClassDefinition& classDef = def.ClassMap.Emplace(classDto.Name);
				classDef.Name = classDto.Name;
				classDef.Flags = static_cast<EObjectFlags>(classDto.Flags);
				classDef.MetadataMap = MoveTemp(classDto.MetadataMap);
				classDef.SuperPath = classDto.SuperPath;
				classDef.PropertyMap = PropertyDtoMap2DefMap(MoveTemp(classDto.PropertyMap));
				classDef.ConfigName = classDto.ConfigName;
				classDef.WithinPath = classDto.WithinPath;
				classDef.ClassFlags = static_cast<EClassFlags>(classDto.ClassFlags);
				classDef.CastFlags = static_cast<EClassCastFlags>(classDto.CastFlags);
				classDef.ImplementedInterfacePaths = MoveTemp(classDto.ImplementedInterfacePaths);
				classDef.FunctionMap = FunctionDtoMap2DefMap(MoveTemp(classDto.FunctionMap));
			}
		}

		{ // @TODO Interfaces
			
		}

		{ // @TODO Delegates
			
		}

		return def;
	}
	
	static void EmitUnrealFieldsForAssembly(const FString& assemblyName, const FString& manifest)
	{
		FZPackageDefinitionDto pakDto;
		if (!FJsonObjectConverter::JsonObjectStringToUStruct<FZPackageDefinitionDto>(manifest, &pakDto))
		{
			UE_LOG(LogZSharpEmit, Fatal, TEXT("Fail to emit unreal field for assembly [%s]!!! Manifest: [%s]"), *assemblyName, *manifest);
			return;
		}

		FZPackageDefinition pakDef = PackageDto2Def(MoveTemp(pakDto));
		FZUnrealFieldEmitter::Get().Emit(pakDef);
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
	
	TArray<FString> assemblies = GetDefault<UZSharpEmitSettings>()->GetModuleAssembliesToScan(moduleName);
	for (const auto& assembly : assemblies)
	{
		if (ScannedAssemblies.Contains(assembly))
		{
			continue;
		}
		ScannedAssemblies.Emplace(assembly);
		
		FString outManifest;
		struct
		{
			const TCHAR* AssemblyName;
			FString* OutManifest;
			uint8 bWithMetadata;
		} args { *assembly, &outManifest, WITH_METADATA };
		ScannerAlc->InvokeMethod(ZSHARP_SCANNER_ASSEMBLY_NAME, "ZeroGames.ZSharp.UnrealFieldScanner.UnrealFieldScanner_Interop", "Scan", &args);

		if (outManifest.IsEmpty())
		{
			continue;
		}

		ZUnrealFieldScanner_Private::EmitUnrealFieldsForAssembly(assembly, outManifest);
	}
}


