// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "CLR/IZSharpClr.h"
#include "ALC/ZMasterAssemblyLoadContext.h"
#include "ALC/ZSlimAssemblyLoadContext.h"

namespace ZSharp
{
	class IZAssemblyLoadContext;
	
	class FZGenericClr final : public IZSharpClr
	{

		using ThisClass = FZGenericClr;

	public:
		static FZGenericClr& Get() { return static_cast<FZGenericClr&>(IZSharpClr::Get()); }

	public:
		void Startup();
		void Shutdown();

	public:
		virtual void CollectGarbage(int32 generation = -1, bool aggressive = true, bool blocking = false, bool compacting = true) override;
		
	public:
		virtual IZMasterAssemblyLoadContext* CreateMasterAlc() override;
		virtual IZMasterAssemblyLoadContext* GetMasterAlc() override;
		virtual IZSlimAssemblyLoadContext* CreateSlimAlc(const FString& name) override;
		virtual IZSlimAssemblyLoadContext* GetSlimAlc(const FString& name) override;

	public:
		virtual FDelegateHandle RegisterMasterAlcLoaded(FZOnMasterAlcLoaded::FDelegate delegate, bool notifyIfLoaded = true) override;
		virtual void UnregisterMasterAlcLoaded(FDelegateHandle delegate) override;
		virtual void UnregisterMasterAlcLoaded(const void* userObject) override;
		virtual FDelegateHandle RegisterMasterAlcLoadFrameworks(FZOnMasterAlcLoadFrameworks::FDelegate delegate, bool notifyIfLoaded = true) override;
		virtual void UnregisterMasterAlcLoadFrameworks(FDelegateHandle delegate) override;
		virtual void UnregisterMasterAlcLoadFrameworks(const void* userObject) override;
		virtual FDelegateHandle RegisterMasterAlcLoadApplications(FZOnMasterAlcLoadApplications::FDelegate delegate, bool notifyIfLoaded = true) override;
		virtual void UnregisterMasterAlcLoadApplications(FDelegateHandle delegate) override;
		virtual void UnregisterMasterAlcLoadApplications(const void* userObject) override;
		virtual FDelegateHandle RegisterMasterAlcLoadPlugins(FZOnMasterAlcLoadPlugins::FDelegate delegate, bool notifyIfLoaded = true) override;
		virtual void UnregisterMasterAlcLoadPlugins(FDelegateHandle delegate) override;
		virtual void UnregisterMasterAlcLoadPlugins(const void* userObject) override;
		virtual FDelegateHandle RegisterMasterAlcFullyLoaded(FZOnMasterAlcFullyLoaded::FDelegate delegate, bool notifyIfLoaded = true) override;
		virtual void UnregisterMasterAlcFullyLoaded(FDelegateHandle delegate) override;
		virtual void UnregisterMasterAlcFullyLoaded(const void* userObject) override;
		virtual FDelegateHandle RegisterMasterAlcUnloaded(FZOnMasterAlcUnloaded::FDelegate delegate) override;
		virtual void UnregisterMasterAlcUnloaded(FDelegateHandle delegate) override;
		virtual void UnregisterMasterAlcUnloaded(const void* userObject) override;

	private:
		void HandleGarbageCollectComplete();
		void HandleMasterAlcUnloaded();
		void HandleSlimAlcUnloaded(const FString& name);

	private:
		bool bInitialized = false;
		TUniquePtr<FZMasterAssemblyLoadContext> MasterAlc;
		FRWLock SlimAlcMapLock;
		TMap<FString, TUniquePtr<FZSlimAssemblyLoadContext>> SlimAlcMap;

	private:
		FZOnMasterAlcLoaded OnMasterAlcLoaded;
		FZOnMasterAlcLoadFrameworks OnMasterAlcLoadFrameworks;
		FZOnMasterAlcLoadApplications OnMasterAlcLoadApplications;
		FZOnMasterAlcLoadPlugins OnMasterAlcLoadPlugins;
		FZOnMasterAlcFullyLoaded OnMasterAlcFullyLoaded;
		FZOnMasterAlcUnloaded OnMasterAlcUnloaded;
		
	};
}


