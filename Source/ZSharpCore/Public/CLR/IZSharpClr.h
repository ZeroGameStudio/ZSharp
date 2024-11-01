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
 	 *
 	 * A .NET runtime will automatically set up based on which implementation you choose when this module is loaded.
 	 * Generally, it uses CoreCLR for desktop platforms, and Mono for other platforms.
 	 * For those platforms who prohibit JIT compilation, interpreter mode will be used.
 	 * The lifecycle of the runtime accompanies the entire process, and there is one and only one CLR object per process.
 	 * The runtime internally maintains multiple System.Runtime.Loader.AssemblyLoadContext instances to achieve environment isolation.
 	 *
 	 * There are three types of ALC in Z# architecture:
 	 *   a. Default ALC: Specifically refers to System.Runtime.Loader.AssemblyLoadContext.Default.
 	 *                   This is the built-in ALC of the .NET runtime, where all assemblies run by default.
 	 *                   You can load assemblies into Default ALC and interact with it simply through reflection.
 	 *                   
 	 *   b. Master ALC:  This is the main ALC of Z#, which features the 'Conjugate' and 'ZCall' mechanisms,
 	 *					 allowing complex interactions between managed and unmanaged code.
 	 *					 The bridge connecting the UObject world and the managed world in Z# is just built on the Master ALC.
 	 *					 Master ALC can be loaded and unloaded repeatedly to support hot reload mechanisms,
 	 *					 but only one instance can exist at any given time.
 	 *					 In most cases, the lifecycle of the Master ALC is managed automatically by Z#.
 	 *					 Of course, you can explicitly load and unload the Master ALC,
 	 *					 but this requires a sufficient understanding of the internals of Z#.
 	 *					 
 	 *   c. Slim ALC:    This is a lightweight ALC with an interface set similar to Default ALC,
 	 *					 allowing only assembly loading and simple calls.
 	 *					 However, Default ALC contains globally shared state,
 	 *					 frequent interaction with Default ALC may lead to unpredictable side effects.
 	 *					 Therefore, Slim ALC comes into play to enable relatively independent code to run in a sandbox,
 	 *					 minimizing the impact on the global environment.
 	 *					 Slim ALC has a name property, allowing multiple instances with different names to coexist.
 	 *					 Slim ALC needs to be explicitly unloaded after use.
 	 *
 	 * Use this interface as an entry point to interact with .NET runtime.
 	 *
 	 * [Library Interface] - you should just consume this and have no reason to implement this.
 	 */
	class IZSharpClr
	{
		
	public:
		/**
		 * Gets the proxy object represents the CLR instance of current process.
		 *
		 * @return The proxy object represents the CLR instance of current process.
		 */
		ZSHARPCORE_API static IZSharpClr& Get();
		
	public:
		/**
		 * Call managed GC.
		 * Parameters have no effect yet.
		 */
		virtual void CollectGarbage(int32 generation = -1, bool aggressive = true, bool blocking = false, bool compacting = true) = 0;

	public:
		/**
		 * Gets the proxy object represents System.Runtime.Loader.AssemblyLoadContext.Default.
		 *
		 * @return The proxy object represents System.Runtime.Loader.AssemblyLoadContext.Default.
		 */
		virtual IZDefaultAssemblyLoadContext& GetDefaultAlc() = 0;

		/**
		 * Creates an instance of ZeroGames.ZSharp.Core.IMasterAssemblyLoadContext.
		 * If the ALC already exists, a warning will be issued.
		 * IMPORTANT: You don't own the returned pointer so don't store it if you don't know what you are doing!
		 *
		 * @return The proxy object represents the newly created or existing ALC.
		 */
		virtual IZMasterAssemblyLoadContext* CreateMasterAlc() = 0;

		/**
		 * Gets the proxy object represents the existing instance of ZeroGames.ZSharp.Core.IMasterAssemblyLoadContext.
		 * IMPORTANT: You don't own the returned pointer so don't store it if you don't know what you are doing!
		 *
		 * @return The proxy object represents the existing instance of ZeroGames.ZSharp.Core.IMasterAssemblyLoadContext.
		 *         If no instance exists, nullptr will be returned.
		 */
		virtual IZMasterAssemblyLoadContext* GetMasterAlc() = 0;

		/**
 		 * Creates an instance of ZeroGames.ZSharp.Core.ISlimAssemblyLoadContext.
 		 * If the input name doesn't conform the contract, a warning will be issued.
 		 * IMPORTANT: You don't own the returned pointer so don't store it if you don't know what you are doing!
 		 *
 		 * @param name Name of the instance. Should be neither "Default" nor "Master". 
 		 * @return The proxy object represents the newly created or existing ALC.
 		 *         If name doesn't conform the contract, nullptr will be returned.
 		 */
		virtual IZSlimAssemblyLoadContext* CreateSlimAlc(const FString& name) = 0;
		
		/**
 		 * Gets the proxy object represents the existing instance of ZeroGames.ZSharp.Core.ISlimAssemblyLoadContext with provided name.
 		 * IMPORTANT: You don't own the returned pointer so don't store it if you don't know what you are doing!
 		 *
 		 * @return The proxy object represents the existing instance of ZeroGames.ZSharp.Core.ISlimAssemblyLoadContext with provided name.
 		 *         If no instance exists or name doesn't conform the contract, nullptr will be returned.
 		 */
		virtual IZSlimAssemblyLoadContext* GetSlimAlc(const FString& name) = 0;
		
	public:
		/**
		 * Gets the event fired immediately after the Master ALC is allocated.
		 *
		 * @return The event fired immediately after the Master ALC is allocated.
		 */
		virtual TMulticastDelegateRegistration<void(IZMasterAssemblyLoadContext*)>& PreMasterAlcStartup() = 0;

		/**
		 * Gets the event fired immediately after PreMasterAlcStartup does.
		 *
		 * @return The event fired immediately after PreMasterAlcStartup does.
		 */
		virtual TMulticastDelegateRegistration<void(IZMasterAssemblyLoadContext*)>& OnMasterAlcStartup() = 0;

		/**
		 * Gets the event fired immediately after OnMasterAlcStartup does.
		 *
		 * @return The event fired immediately after OnMasterAlcStartup does.
		 */
		virtual TMulticastDelegateRegistration<void(IZMasterAssemblyLoadContext*)>& PostMasterAlcStartup() = 0;

		/**
		 * Gets the event fired immediately after PostMasterAlcStartup does.
		 *
		 * @return The event fired immediately after PostMasterAlcStartup does.
		 */
		virtual TMulticastDelegateRegistration<void(IZMasterAssemblyLoadContext*)>& OnMasterAlcLoaded() = 0;

		/**
		 * Gets the event fired just before Master ALC unloads and releases any resource.
		 *
		 * @return The event fired just before Master ALC unloads and releases any resource.
		 */
		virtual TMulticastDelegateRegistration<void(IZMasterAssemblyLoadContext*)>& OnMasterAlcUnloading() = 0;

		/**
		 * Gets the event fired immediately after ALC has finished unloading.
		 *
		 * @return The event fired immediately after ALC has finished unloading.
		 */
		virtual TMulticastDelegateRegistration<void()>& OnMasterAlcUnloaded() = 0;

		/**
		 * Binds to OnMasterAlcLoaded.
		 * If Master ALC already exists, the input delegate will be fired inplace.
		 *
		 * @return The handle represents the bound delegate.
		 */
		virtual FDelegateHandle CallOrRegisterOnMasterAlcLoaded(FZOnMasterAlcLoaded::FDelegate delegate) = 0;

	public:
		/**
		 * Creates an anonymous Slim ALC, load the specified assembly, and call DllMain.
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
 		 * Creates an anonymous Slim ALC, load the specified assembly, and call DllMain.
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

	static_assert(sizeof(TCHAR) == 2, "Z# expects that TCHAR is 16-bit.");
}



