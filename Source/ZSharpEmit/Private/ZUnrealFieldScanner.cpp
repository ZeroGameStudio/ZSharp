// Copyright Zero Games. All Rights Reserved.


#include "ZUnrealFieldScanner.h"

#include "ZSharpEmitLogChannels.h"
#include "ZSharpEmitSettings.h"
#include "ALC/IZSlimAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"

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
		FString outManifest;
		struct
		{
			const TCHAR* assemblyName;
			FString* outManifest;
		} args { *assembly, &outManifest };
		ScannerAlc->InvokeMethod(ZSHARP_SCANNER_ASSEMBLY_NAME, "ZeroGames.ZSharp.UnrealFieldScanner.UnrealFieldScanner_Interop", "Scan", &args);
		UE_LOG(LogZSharpEmit, Log, TEXT("%s"), *outManifest);
	}
}


