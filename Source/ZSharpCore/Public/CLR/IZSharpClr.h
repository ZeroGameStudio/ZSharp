// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	class IZMasterAssemblyLoadContext;
	class IZSlimAssemblyLoadContext;

	DECLARE_MULTICAST_DELEGATE_OneParam(FZOnMasterAlcLoaded, IZMasterAssemblyLoadContext*)
	DECLARE_MULTICAST_DELEGATE_OneParam(FZOnMasterAlcLoadFrameworks, IZMasterAssemblyLoadContext*)
	DECLARE_MULTICAST_DELEGATE_OneParam(FZOnMasterAlcLoadApplications, IZMasterAssemblyLoadContext*)
	DECLARE_MULTICAST_DELEGATE_OneParam(FZOnMasterAlcLoadPlugins, IZMasterAssemblyLoadContext*)
	DECLARE_MULTICAST_DELEGATE_OneParam(FZOnMasterAlcFullyLoaded, IZMasterAssemblyLoadContext*)
	DECLARE_MULTICAST_DELEGATE(FZOnMasterAlcUnloaded)
	
	class ZSHARPCORE_API IZSharpClr
	{
	public:
		static IZSharpClr& Get();
	public:
		virtual ~IZSharpClr(){}
	public:
		virtual void CollectGarbage(int32 generation = -1, bool aggressive = true, bool blocking = false, bool compacting = true) = 0;
	public:
		virtual IZMasterAssemblyLoadContext* CreateMasterAlc() = 0;
		virtual IZMasterAssemblyLoadContext* GetMasterAlc() = 0;
		virtual IZSlimAssemblyLoadContext* CreateSlimAlc(const FString& name) = 0;
		virtual IZSlimAssemblyLoadContext* GetSlimAlc(const FString& name) = 0;
	public:
		virtual FDelegateHandle RegisterMasterAlcLoaded(FZOnMasterAlcLoaded::FDelegate delegate, bool notifyIfLoaded = true) = 0;
		virtual void UnregisterMasterAlcLoaded(FDelegateHandle delegate) = 0;
		virtual void UnregisterMasterAlcLoaded(const void* userObject) = 0;
		virtual FDelegateHandle RegisterMasterAlcLoadFrameworks(FZOnMasterAlcLoadFrameworks::FDelegate delegate, bool notifyIfLoaded = true) = 0;
		virtual void UnregisterMasterAlcLoadFrameworks(FDelegateHandle delegate) = 0;
		virtual void UnregisterMasterAlcLoadFrameworks(const void* userObject) = 0;
		virtual FDelegateHandle RegisterMasterAlcLoadApplications(FZOnMasterAlcLoadApplications::FDelegate delegate, bool notifyIfLoaded = true) = 0;
		virtual void UnregisterMasterAlcLoadApplications(FDelegateHandle delegate) = 0;
		virtual void UnregisterMasterAlcLoadApplications(const void* userObject) = 0;
		virtual FDelegateHandle RegisterMasterAlcLoadPlugins(FZOnMasterAlcLoadPlugins::FDelegate delegate, bool notifyIfLoaded = true) = 0;
		virtual void UnregisterMasterAlcLoadPlugins(FDelegateHandle delegate) = 0;
		virtual void UnregisterMasterAlcLoadPlugins(const void* userObject) = 0;
		virtual FDelegateHandle RegisterMasterAlcFullyLoaded(FZOnMasterAlcFullyLoaded::FDelegate delegate, bool notifyIfLoaded = true) = 0;
		virtual void UnregisterMasterAlcFullyLoaded(FDelegateHandle delegate) = 0;
		virtual void UnregisterMasterAlcFullyLoaded(const void* userObject) = 0;
		virtual FDelegateHandle RegisterMasterAlcUnloaded(FZOnMasterAlcUnloaded::FDelegate delegate) = 0;
		virtual void UnregisterMasterAlcUnloaded(FDelegateHandle delegate) = 0;
		virtual void UnregisterMasterAlcUnloaded(const void* userObject) = 0;
		// Helpers
	public:
		int32 Run(const FString& path, void* args = nullptr, const FString& alcName = "");
		int32 RunAsync(const FString& path, void* args = nullptr, const FString& alcName = "");
	};
}



