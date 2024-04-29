// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "CLR/IZSharpCLR.h"
#include "ALC/IZMasterAssemblyLoadContext.h"
#include "ALC/IZSlimAssemblyLoadContext.h"

namespace ZSharp
{
	class IZAssemblyLoadContext;
	
	class FZGenericCLR : public IZSharpCLR
	{

		using ThisClass = FZGenericCLR;

	public:
		static FZGenericCLR& Get() { return StaticCast<FZGenericCLR&>(IZSharpCLR::Get()); }

	public:
		void Startup();
		void Shutdown();

	public:
		virtual void CollectGarbage(int32 generation = -1, bool bAggressive = true, bool bBlocking = false, bool bCompacting = true) override;
		
	public:
		virtual IZMasterAssemblyLoadContext* CreateMasterALC() override;
		virtual IZMasterAssemblyLoadContext* GetMasterALC() override;
		virtual void GetMasterALC_AnyThread(TFunctionRef<void(IZMasterAssemblyLoadContext*)> action) override;
		virtual IZSlimAssemblyLoadContext* CreateSlimALC(const FString& name) override;
		virtual IZSlimAssemblyLoadContext* GetSlimALC(const FString& name) override;

	public:
		virtual FDelegateHandle RegisterMasterALCLoaded(FZOnMasterALCLoaded::FDelegate delegate, bool bNotifyIfLoaded = true) override;
		virtual void UnregisterMasterALCLoaded(FDelegateHandle delegate) override;
		virtual void UnregisterMasterALCLoaded(const void* userObject) override;
		virtual FDelegateHandle RegisterMasterALCUnloaded(FZOnMasterALCUnloaded::FDelegate delegate) override;
		virtual void UnregisterMasterALCUnloaded(FDelegateHandle delegate) override;
		virtual void UnregisterMasterALCUnloaded(const void* userObject) override;

	private:
		void HandleGarbageCollectComplete();
		void HandleMasterALCUnloaded();
		void HandleSlimALCUnloaded(const FString& name);

	private:
		bool bInitialized = false;
		FCriticalSection MasterALCCriticalSection;
		TUniquePtr<IZMasterAssemblyLoadContext> MasterALC;
		FRWLock SlimALCMapLock;
		TMap<FString, TUniquePtr<IZSlimAssemblyLoadContext>> SlimALCMap;

	private:
		FZOnMasterALCLoaded OnMasterALCLoaded;
		FZOnMasterALCUnloaded OnMasterALCUnloaded;
		
	};
}


