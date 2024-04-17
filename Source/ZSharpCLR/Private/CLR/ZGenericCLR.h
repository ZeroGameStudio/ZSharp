// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "CLR/IZCLR.h"
#include "Interop/IZMasterAssemblyLoadContext.h"

namespace ZSharp
{
	class IZAssemblyLoadContext;
	
	class FZGenericCLR : public IZCLR
	{

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
		virtual IZMasterAssemblyLoadContext* LoadMasterALC() override;
		virtual void UnloadMasterALC() override;
		virtual IZMasterAssemblyLoadContext* GetMasterALC() override;
		virtual IZSlimAssemblyLoadContext* LoadSlimALC(const FString& name) override;
		virtual void UnloadSlimALC(const FString& name) override;
		virtual void UnloadSlimALC(IZSlimAssemblyLoadContext* alc) override;
		virtual IZSlimAssemblyLoadContext* GetSlimALC(const FString& name) override;

	private:
		FZOnMasterALCLoaded OnMasterALCLoaded;
		FZOnMasterALCUnloaded OnMasterALCUnloaded;

	private:
		void HandleMasterALCUnloaded();

	private:
		bool bInitialized = false;
		TUniquePtr<IZMasterAssemblyLoadContext> MasterALC;
		
	};
}


