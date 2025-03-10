// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSharpRuntimeSettings.h"

namespace ZSharp
{
	class IZSlimAssemblyLoadContext;
	
	class FZUnrealFieldScanner
	{

		using ThisClass = FZUnrealFieldScanner;
		
	public:
		static FZUnrealFieldScanner& Get();

	public:
		void Startup();
		void Shutdown();

	private:
		FZUnrealFieldScanner() = default;

	private:
		void ProcessVirtualModules();
		void FlushDeferredModules();
		void ScanUnrealFieldsForModule(FName moduleName, bool canProcessNewlyLoadedObject);
		
	private:
		FDelegateHandle PostEngineInitDelegate;
		FDelegateHandle ProcessLoadedObjectsDelegate;

		TOptional<TMultiMap<EZEmitVirtualModuleLoadingPhase, FZEmitVirtualModule>> VirtualModuleLookup;
		TMultiMap<FName, FZEmitVirtualModule> VirtualModuleTargetLookup;
		
		TOptional<TArray<FName>> DeferredModules;
		TSet<FName> ScannedModules;
		
	};
}


