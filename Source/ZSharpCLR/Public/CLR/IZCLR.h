// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	class IZMasterAssemblyLoadContext;
	class IZSlimAssemblyLoadContext;

	DECLARE_MULTICAST_DELEGATE_OneParam(FZOnMasterALCLoaded, IZMasterAssemblyLoadContext*)
	DECLARE_MULTICAST_DELEGATE(FZOnMasterALCUnloaded)
	
	class ZSHARPCLR_API IZCLR
	{
	public:
		static IZCLR& Get();
	public:
		virtual ~IZCLR(){}
	public:
		virtual IZMasterAssemblyLoadContext* LoadMasterALC() = 0;
		virtual void UnloadMasterALC() = 0;
		virtual IZMasterAssemblyLoadContext* GetMasterALC() = 0;
		virtual IZSlimAssemblyLoadContext* LoadSlimALC(const FString& name) = 0;
		virtual void UnloadSlimALC(IZSlimAssemblyLoadContext* alc) = 0;
		virtual void UnloadSlimALC(const FString& name) = 0;
		virtual IZSlimAssemblyLoadContext* GetSlimALC(const FString& name) = 0;
	public:
		virtual FDelegateHandle RegisterMasterALCLoaded(FZOnMasterALCLoaded::FDelegate delegate, bool bNotifyIfLoaded = true) = 0;
		virtual void UnregisterMasterALCLoaded(FDelegateHandle delegate) = 0;
		virtual void UnregisterMasterALCLoaded(const void* userObject) = 0;
		virtual FDelegateHandle RegisterMasterALCUnloaded(FZOnMasterALCUnloaded::FDelegate delegate) = 0;
		virtual void UnregisterMasterALCUnloaded(FDelegateHandle delegate) = 0;
		virtual void UnregisterMasterALCUnloaded(const void* userObject) = 0;
	};
}



