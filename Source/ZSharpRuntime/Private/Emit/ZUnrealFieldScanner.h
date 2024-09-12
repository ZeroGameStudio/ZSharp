// Copyright Zero Games. All Rights Reserved.

#pragma once

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
		void FlushDeferredModules();
		void ScanUnrealFieldsForModule(FName moduleName, bool canProcessNewlyLoadedObject);
		
	private:
		IZSlimAssemblyLoadContext* ScannerAlc = nullptr;
		FDelegateHandle ProcessLoadedObjectsDelegate;
		
		TOptional<TArray<FName>> DeferredModules;
		TSet<FName> ScannedModules;
		
	};
}


