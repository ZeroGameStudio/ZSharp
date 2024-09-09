// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	class IZSlimAssemblyLoadContext;
	
	class FZUnrealFieldScanner
	{
		
	public:
		static FZUnrealFieldScanner& Get();

	public:
		void Startup();
		void Shutdown();

	private:
		FZUnrealFieldScanner() = default;

	private:
		void ScanUnrealFieldsForModule(FName moduleName, bool canProcessNewlyLoadedObject);
		void FlushDeferredModules();

	private:
		IZSlimAssemblyLoadContext* ScannerAlc = nullptr;
		FDelegateHandle ProcessLoadedObjectsDelegate;
		
		TOptional<TArray<FName>> DeferredModules;
		TSet<FString> ScannedAssemblies;
		
	};
}


