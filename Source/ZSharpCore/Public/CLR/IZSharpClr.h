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
	DECLARE_MULTICAST_DELEGATE_OneParam(FZOnMasterAlcUnloading, IZMasterAssemblyLoadContext*)
	DECLARE_MULTICAST_DELEGATE(FZOnMasterAlcUnloaded)

	/**
 	 * Represents a running CLR.
 	 * A CLR object is automatically created when the module is loaded based on the configuration.
 	 * The lifecycle of the CLR object accompanies the entire process, and there is one and only one CLR object per process.
 	 * The CLR object internally maintains multiple AssemblyLoadContexts to achieve environment isolation.
 	 *
 	 * @see IZAssemblyLoadContext
 	 */
	class IZSharpClr
	{
		
	public:
		/** Get a proxy object for the CLR instance of current process. */
		ZSHARPCORE_API static IZSharpClr& Get();
		
	public:
		/**
		 * Call managed GC.
		 * Parameters have no effect yet.
		 */
		virtual void CollectGarbage(int32 generation = -1, bool aggressive = true, bool blocking = false, bool compacting = true) = 0;

	public:
		/** Get a proxy object for System.Runtime.Loader.AssemblyLoadContext.Default. */
		virtual IZDefaultAssemblyLoadContext& GetDefaultAlc() = 0;

		/**
		 * Create an instance of ZeroGames.ZSharp.Core.IMasterAssemblyLoadContext.
		 * If the ALC already exists, a warning will be issued.
		 * IMPORTANT: You don't own the returned pointer so don't store it if you don't know what you are doing!
		 * 
		 * @return A proxy object for the newly created or existing ALC.
		 */
		virtual IZMasterAssemblyLoadContext* CreateMasterAlc() = 0;

		/**
		 * Get a proxy object for existing instance of ZeroGames.ZSharp.Core.IMasterAssemblyLoadContext.
		 * If no instance exists, nullptr will be returned.
		 * IMPORTANT: You don't own the returned pointer so don't store it if you don't know what you are doing!
		 */
		virtual IZMasterAssemblyLoadContext* GetMasterAlc() = 0;

		/**
 		 * Create an instance of ZeroGames.ZSharp.Core.ISlimAssemblyLoadContext.
 		 * If the input name doesn't conform the contract, a warning will be issued.
 		 * IMPORTANT: You don't own the returned pointer so don't store it if you don't know what you are doing!
 		 *
 		 * @param name Name of the instance. Should be neither "Default" nor "Master". 
 		 * @return A proxy object for the newly created or existing ALC.
 		 *		   If name doesn't conform the contract, nullptr will be returned.
 		 */
		virtual IZSlimAssemblyLoadContext* CreateSlimAlc(const FString& name) = 0;
		
		/**
 		 * Get a proxy object for existing instance of ZeroGames.ZSharp.Core.ISlimAssemblyLoadContext with provided name.
 		 * If no instance exists or name doesn't conform the contract, nullptr will be returned.
 		 * IMPORTANT: You don't own the returned pointer so don't store it if you don't know what you are doing!
 		 */
		virtual IZSlimAssemblyLoadContext* GetSlimAlc(const FString& name) = 0;
		
	public:
		/** Event fired immediately after the Master ALC is allocated. */
		virtual TMulticastDelegateRegistration<void(IZMasterAssemblyLoadContext*)>& PreMasterAlcStartup() = 0;

		/** Event fired immediately after PreMasterAlcStartup does. */
		virtual TMulticastDelegateRegistration<void(IZMasterAssemblyLoadContext*)>& OnMasterAlcStartup() = 0;

		/** Event fired immediately after OnMasterAlcStartup does. */
		virtual TMulticastDelegateRegistration<void(IZMasterAssemblyLoadContext*)>& PostMasterAlcStartup() = 0;

		/** Event fired immediately after PostMasterAlcStartup does. */
		virtual TMulticastDelegateRegistration<void(IZMasterAssemblyLoadContext*)>& OnMasterAlcLoaded() = 0;

		/** Event fired just before Master ALC unloads and releases any resource. */
		virtual TMulticastDelegateRegistration<void(IZMasterAssemblyLoadContext*)>& OnMasterAlcUnloading() = 0;

		/** Event fired immediately after ALC has finished unloading. */
		virtual TMulticastDelegateRegistration<void()>& OnMasterAlcUnloaded() = 0;

		/**
		 * Bind to OnMasterAlcLoaded.
		 * If Master ALC already exists, the input delegate will be fired inplace.
		 */
		virtual FDelegateHandle CallOrRegisterOnMasterAlcLoaded(FZOnMasterAlcLoaded::FDelegate delegate) = 0;

	public:
		/**
		 * Create an anonymous Slim ALC, load the specified assembly, and call DllMain.
		 * This function assumes the user function is synchronous,
		 * so the ALC created will be immediately unloaded after the assembly is loaded.
		 * If the assembly doesn't exist, or doesn't have DllEntry, nothing happens.
		 *
		 * @param name Name of the pending assembly.
		 * @param args A transparent argument to pass to DllMain method.
		 * @param alcName An optional name to assign to the Slim ALC.
		 * @return 0 if succeeded, otherwise failed.
		 */
		ZSHARPCORE_API int32 Run(const FString& name, void* args = nullptr, const FString& alcName = "");

		/**
 		 * Create an anonymous Slim ALC, load the specified assembly, and call DllMain.
 		 * This function assumes the user function is asynchronous,
 		 * so the ALC created won't be unloaded automatically and you are responsible for unloading it.
 		 * If the assembly doesn't exist, or doesn't have DllEntry, nothing happens.
 		 *
 		 * @param name Name of the pending assembly.
 		 * @param args A transparent argument to pass to DllMain method.
 		 * @param alcName An optional name to assign to the Slim ALC.
 		 * @return 0 if succeeded, otherwise failed.
 		 */
		ZSHARPCORE_API int32 RunAsync(const FString& name, void* args = nullptr, const FString& alcName = "");

	public:
		virtual ~IZSharpClr(){}
		
	};
}



