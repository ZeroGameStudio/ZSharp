// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	class IZDefaultAssemblyLoadContext;
	class IZMasterAssemblyLoadContext;
	class IZSlimAssemblyLoadContext;

	DECLARE_MULTICAST_DELEGATE_OneParam(FZOnMasterAlcLoaded, IZMasterAssemblyLoadContext*)
	DECLARE_MULTICAST_DELEGATE_OneParam(FZOnMasterAlcLoadFrameworks, IZMasterAssemblyLoadContext*)
	DECLARE_MULTICAST_DELEGATE_OneParam(FZOnMasterAlcLoadApplications, IZMasterAssemblyLoadContext*)
	DECLARE_MULTICAST_DELEGATE_OneParam(FZOnMasterAlcLoadPlugins, IZMasterAssemblyLoadContext*)
	DECLARE_MULTICAST_DELEGATE_OneParam(FZOnMasterAlcFullyLoaded, IZMasterAssemblyLoadContext*)
	DECLARE_MULTICAST_DELEGATE(FZOnMasterAlcUnloaded)

	/**
 	 * Represents a running CLR.
 	 * A CLR object is automatically created when the module is loaded based on the configuration.
 	 * The lifecycle of the CLR object accompanies the entire process, and there is one and only one CLR object per process.
 	 * The CLR object internally maintains multiple AssemblyLoadContexts to achieve environment isolation.
 	 *
 	 * @see IZAssemblyLoadContext
 	 */
	class ZSHARPCORE_API IZSharpClr
	{
	public:
		/** Get the unique CLR object for the current process. */
		static IZSharpClr& Get();
		
	public:
		/** Call managed GC. */
		virtual void CollectGarbage(int32 generation = -1, bool aggressive = true, bool blocking = false, bool compacting = true) = 0;
	public:
		virtual IZDefaultAssemblyLoadContext& GetDefaultAlc() = 0;
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
		int32 Run(const FString& name, void* args = nullptr, const FString& alcName = "");
		int32 RunAsync(const FString& name, void* args = nullptr, const FString& alcName = "");

	public:
		virtual ~IZSharpClr(){}
	};
}



