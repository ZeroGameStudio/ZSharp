// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "CLR/IZSharpClr.h"
#include "ALC/ZDefaultAssemblyLoadContext.h"
#include "ALC/ZMasterAssemblyLoadContext.h"
#include "ALC/ZSlimAssemblyLoadContext.h"

namespace ZSharp
{
	class IZAssemblyLoadContext;
	
	class FZGenericClr final : public IZSharpClr
	{

		using ThisClass = FZGenericClr;

	public:
		static FZGenericClr& Get();

	public:
		void Startup();
		void Shutdown();

	public:
		virtual void CollectGarbage(int32 generation = -1, bool aggressive = true, bool blocking = false, bool compacting = true) override;
		
	public:
		virtual IZDefaultAssemblyLoadContext& GetDefaultAlc() override;
		virtual IZMasterAssemblyLoadContext* CreateMasterAlc() override;
		virtual IZMasterAssemblyLoadContext* GetMasterAlc() override;
		virtual IZSlimAssemblyLoadContext* CreateSlimAlc(const FString& name) override;
		virtual IZSlimAssemblyLoadContext* GetSlimAlc(const FString& name) override;

	public:
		virtual TMulticastDelegateRegistration<void(IZMasterAssemblyLoadContext*)>& PreMasterAlcStartup() override { return PreMasterAlcStartupDelegate; }
		virtual TMulticastDelegateRegistration<void(IZMasterAssemblyLoadContext*)>& OnMasterAlcStartup() override { return OnMasterAlcStartupDelegate; }
		virtual TMulticastDelegateRegistration<void(IZMasterAssemblyLoadContext*)>& PostMasterAlcStartup() override { return PostMasterAlcStartupDelegate; }
		virtual TMulticastDelegateRegistration<void(IZMasterAssemblyLoadContext*)>& OnMasterAlcLoaded() override { return OnMasterAlcLoadedDelegate; }
		virtual TMulticastDelegateRegistration<void()>& OnMasterAlcUnloaded() override { return OnMasterAlcUnloadedDelegate; }
		virtual FDelegateHandle CallOrRegisterOnMasterAlcLoaded(FZOnMasterAlcLoaded::FDelegate delegate) override;
		
	private:
		FZGenericClr() = default;

	private:
		void HandleGarbageCollectComplete();
		void HandleMasterAlcUnloaded();
		void HandleSlimAlcUnloaded(const FString& name);

	private:
		bool bInitialized = false;
		TUniquePtr<FZDefaultAssemblyLoadContext> DefaultAlc;
		TUniquePtr<FZMasterAssemblyLoadContext> MasterAlc;
		FRWLock SlimAlcMapLock;
		TMap<FString, TUniquePtr<FZSlimAssemblyLoadContext>> SlimAlcMap;

	private:
		FZPreMasterAlcStartup PreMasterAlcStartupDelegate;
		FZOnMasterAlcStartup OnMasterAlcStartupDelegate;
		FZPostMasterAlcStartup PostMasterAlcStartupDelegate;
		FZOnMasterAlcLoaded OnMasterAlcLoadedDelegate;
		FZOnMasterAlcUnloaded OnMasterAlcUnloadedDelegate;
		
	};
}


