// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	class IZDefaultAssemblyLoadContext;
	class IZMasterAssemblyLoadContext;
	class IZSlimAssemblyLoadContext;

	DECLARE_MULTICAST_DELEGATE_OneParam(FZPreMasterAlcStartup, IZMasterAssemblyLoadContext*)
	DECLARE_MULTICAST_DELEGATE_OneParam(FZOnMasterAlcStartup, IZMasterAssemblyLoadContext*)
	DECLARE_MULTICAST_DELEGATE_OneParam(FZPostMasterAlcStartup, IZMasterAssemblyLoadContext*)
	DECLARE_MULTICAST_DELEGATE_OneParam(FZOnMasterAlcLoaded, IZMasterAssemblyLoadContext*)
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
		virtual TMulticastDelegateRegistration<void(IZMasterAssemblyLoadContext*)>& PreMasterAlcStartup() = 0;
		virtual TMulticastDelegateRegistration<void(IZMasterAssemblyLoadContext*)>& OnMasterAlcStartup() = 0;
		virtual TMulticastDelegateRegistration<void(IZMasterAssemblyLoadContext*)>& PostMasterAlcStartup() = 0;
		virtual TMulticastDelegateRegistration<void(IZMasterAssemblyLoadContext*)>& OnMasterAlcLoaded() = 0;
		virtual TMulticastDelegateRegistration<void()>& OnMasterAlcUnloaded() = 0;
		virtual FDelegateHandle CallOrRegisterOnMasterAlcLoaded(FZOnMasterAlcLoaded::FDelegate delegate) = 0;
		// Helpers
	public:
		int32 Run(const FString& name, void* args = nullptr, const FString& alcName = "");
		int32 RunAsync(const FString& name, void* args = nullptr, const FString& alcName = "");

	public:
		virtual ~IZSharpClr(){}
	};
}



