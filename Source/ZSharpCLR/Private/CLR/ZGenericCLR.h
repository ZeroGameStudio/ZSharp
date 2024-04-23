// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "CLR/IZSharpCLR.h"
#include "Interop/IZMasterAssemblyLoadContext.h"
#include "Interop/IZSlimAssemblyLoadContext.h"

namespace ZSharp
{
	class IZAssemblyLoadContext;
	
	class FZGenericCLR : public IZSharpCLR
	{

	public:
		static FZGenericCLR& Get() { return (FZGenericCLR&)IZSharpCLR::Get(); }

	public:
		virtual FDelegateHandle RegisterMasterALCLoaded(FZOnMasterALCLoaded::FDelegate delegate, bool bNotifyIfLoaded = true) override;
		virtual void UnregisterMasterALCLoaded(FDelegateHandle delegate) override;
		virtual void UnregisterMasterALCLoaded(const void* userObject) override;
		virtual FDelegateHandle RegisterMasterALCUnloaded(FZOnMasterALCUnloaded::FDelegate delegate) override;
		virtual void UnregisterMasterALCUnloaded(FDelegateHandle delegate) override;
		virtual void UnregisterMasterALCUnloaded(const void* userObject) override;
		
	public:
		void Startup();

	public:
		virtual IZMasterAssemblyLoadContext* CreateMasterALC() override;
		virtual void UnloadMasterALC() override;
		virtual IZMasterAssemblyLoadContext* GetMasterALC() override;
		virtual IZSlimAssemblyLoadContext* CreateSlimALC(const FString& name) override;
		virtual void UnloadSlimALC(const FString& name) override;
		virtual void UnloadSlimALC(IZSlimAssemblyLoadContext* alc) override;
		virtual IZSlimAssemblyLoadContext* GetSlimALC(const FString& name) override;

	private:
		FZOnMasterALCLoaded OnMasterALCLoaded;
		FZOnMasterALCUnloaded OnMasterALCUnloaded;

	private:
		void HandleMasterALCUnloaded();
		void HandleSlimALCUnloaded(const FString& name);

	private:
		bool bInitialized = false;
		TUniquePtr<IZMasterAssemblyLoadContext> MasterALC;
		TMap<FString, TUniquePtr<IZSlimAssemblyLoadContext>> SlimALCMap;
		
	};
}


