// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "CLR/IZSharpClr.h"
#include "ALC/IZMasterAssemblyLoadContext.h"
#include "ALC/IZSlimAssemblyLoadContext.h"

namespace ZSharp
{
	class IZAssemblyLoadContext;
	
	class FZGenericClr : public IZSharpClr
	{

		using ThisClass = FZGenericClr;

	public:
		static FZGenericClr& Get() { return StaticCast<FZGenericClr&>(IZSharpClr::Get()); }

	public:
		void Startup();
		void Shutdown();

	public:
		virtual void CollectGarbage(int32 generation = -1, bool bAggressive = true, bool bBlocking = false, bool bCompacting = true) override;
		
	public:
		virtual IZMasterAssemblyLoadContext* CreateMasterAlc() override;
		virtual IZMasterAssemblyLoadContext* GetMasterAlc() override;
		virtual IZSlimAssemblyLoadContext* CreateSlimAlc(const FString& name) override;
		virtual IZSlimAssemblyLoadContext* GetSlimAlc(const FString& name) override;

	public:
		virtual FDelegateHandle RegisterMasterAlcLoaded(FZOnMasterAlcLoaded::FDelegate delegate, bool bNotifyIfLoaded = true) override;
		virtual void UnregisterMasterAlcLoaded(FDelegateHandle delegate) override;
		virtual void UnregisterMasterAlcLoaded(const void* userObject) override;
		virtual FDelegateHandle RegisterMasterAlcUnloaded(FZOnMasterAlcUnloaded::FDelegate delegate) override;
		virtual void UnregisterMasterAlcUnloaded(FDelegateHandle delegate) override;
		virtual void UnregisterMasterAlcUnloaded(const void* userObject) override;

	private:
		void HandleGarbageCollectComplete();
		void HandleMasterAlcUnloaded();
		void HandleSlimAlcUnloaded(const FString& name);

	private:
		bool bInitialized = false;
		TUniquePtr<IZMasterAssemblyLoadContext> MasterAlc;
		FRWLock SlimAlcMapLock;
		TMap<FString, TUniquePtr<IZSlimAssemblyLoadContext>> SlimAlcMap;

	private:
		FZOnMasterAlcLoaded OnMasterAlcLoaded;
		FZOnMasterAlcUnloaded OnMasterAlcUnloaded;
		
	};
}


