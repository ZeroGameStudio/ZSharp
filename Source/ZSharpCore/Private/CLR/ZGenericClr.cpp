// Copyright Zero Games. All Rights Reserved.

#include "CLR/ZGenericClr.h"

#include "hostfxr.h"
#include "coreclr_delegates.h"
#include "ZSharpClr.h"
#include "ZSharpCoreLogChannels.h"
#include "ALC/IZSlimAssemblyLoadContext.h"
#include "Interop/Engine/Console_Interop.h"
#include "Interop/Core/ZCoreLog_Interop.h"
#include "Interop/Engine/ZLog_Interop.h"
#include "Interop/Core/ZCoreSettings_Interop.h"
#include "Interop/Core/ZClr_Interop.h"
#include "Interop/Core/ZGCHandle_Interop.h"
#include "Interop/Misc/ZInteropString_Interop.h"
#include "Interop/Misc/ZInteropStringArray_Interop.h"
#include "ALC/ZMasterAssemblyLoadContext.h"
#include "Interop/Core/ZMasterAssemblyLoadContext_Interop.h"
#include "Interop/ZGCHandle.h"
#include "ALC/ZSlimAssemblyLoadContext.h"
#include "Interop/Engine/ZConfig_Interop.h"
#include "Interop/Core/ZDefaultAssemblyLoadContext_Interop.h"
#include "Interop/Async/ZEventLoop_Interop.h"
#include "Interop/Core/ZSlimAssemblyLoadContext_Interop.h"
#include "Interop/Engine/ZBuild_Interop.h"
#include "Interop/Engine/ZPath_Interop.h"
#include "Interop/ZUnmanagedFunctionInteropHelper.h"

namespace ZSharp::ZGenericClr_Private
{
	static FAutoConsoleCommand GCmdFullGC
	{
		TEXT("z#.gc"),
		TEXT("Perform a full managed GC."),
		FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>&){ FZSharpClr::Get().CollectGarbage(); }),
		ECVF_Default
	};
	
	static TAutoConsoleVariable<bool> GCVarPerformManagedGCAfterUnmanagedGC
	{
		TEXT("z#.PerformManagedGCAfterUnmanagedGC"),
		false,
		TEXT("If enabled, perform a full managed GC after unmanaged GC.")
	};

	static void HandleError(const TCHAR* error)
	{
		UE_LOG(LogZSharpCore, Error, TEXT("%s"), error);
	}

	static void LoadAssembliesUnderDirectory(const FString& directory, load_assembly_bytes_fn loadAssembly)
	{
		const FString fullDirectory = FPaths::Combine(FPaths::ProjectDir(), "Binaries", "Managed", directory);
		
		TArray<FString> sharedDllFiles;
		IFileManager::Get().FindFilesRecursive(sharedDllFiles, *fullDirectory, TEXT("*.dll"), true, false);

		TArray<uint8> content;
		for (const auto& dll : sharedDllFiles)
		{
			content.Empty();
			if (FFileHelper::LoadFileToArray(content, *dll, FILEREAD_Silent))
			{
				loadAssembly(content.GetData(), content.Num(), nullptr, 0, nullptr, nullptr);
			}
		}
	}

	static void LoadCoreAssembly(load_assembly_bytes_fn loadAssembly, get_function_pointer_fn getFunctionPointer)
	{
		static FZUnmanagedFunction GUnmanagedFunctions[] =
        {
#define ZSHARP_UNMANAGED_FUNCTION_ASSEMBLY ZeroGames.ZSharp.Core
        
			ZSHARP_BUILD_UNMANAGED_FUNCTION(CoreLog, Log),
			
			ZSHARP_BUILD_UNMANAGED_FUNCTION(CoreSettings, ShouldTreatManagedFatalAsError),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(CoreSettings, ShouldSuppressAlcUnloadedException),
			
			ZSHARP_BUILD_UNMANAGED_FUNCTION(InteropString, Alloc),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(InteropString, Free),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(InteropString, GetData),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(InteropString, SetData),

			ZSHARP_BUILD_UNMANAGED_FUNCTION(InteropStringArray, Alloc),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(InteropStringArray, Free),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(InteropStringArray, Count),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(InteropStringArray, Get),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(InteropStringArray, Set),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(InteropStringArray, Add),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(InteropStringArray, InsertAt),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(InteropStringArray, RemoveAt),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(InteropStringArray, Clear),

			ZSHARP_BUILD_UNMANAGED_FUNCTION(MasterAssemblyLoadContext, ZCall_Black),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(MasterAssemblyLoadContext, GetZCallHandle_Black),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(MasterAssemblyLoadContext, BuildConjugate_Black),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(MasterAssemblyLoadContext, ReleaseConjugate_Black),

#undef ZSHARP_UNMANAGED_FUNCTION_ASSEMBLY
        };

		static void** GManagedFunctions[] =
		{
			ZSHARP_BUILD_MANAGED_FUNCTION(FZClr_Interop::GCollectGarbage),
			ZSHARP_BUILD_MANAGED_FUNCTION(FZClr_Interop::GCreateMasterAlc),
			ZSHARP_BUILD_MANAGED_FUNCTION(FZClr_Interop::GCreateSlimAlc),

			ZSHARP_BUILD_MANAGED_FUNCTION(FZGCHandle_Interop::GFree),

			ZSHARP_BUILD_MANAGED_FUNCTION(FZDefaultAssemblyLoadContext_Interop::GLoadAssembly),
			ZSHARP_BUILD_MANAGED_FUNCTION(FZDefaultAssemblyLoadContext_Interop::GInvokeMethod),

			ZSHARP_BUILD_MANAGED_FUNCTION(FZMasterAssemblyLoadContext_Interop::GUnload),
			ZSHARP_BUILD_MANAGED_FUNCTION(FZMasterAssemblyLoadContext_Interop::GLoadAssembly),
			ZSHARP_BUILD_MANAGED_FUNCTION(FZMasterAssemblyLoadContext_Interop::GInvokeMethod),
			ZSHARP_BUILD_MANAGED_FUNCTION(FZMasterAssemblyLoadContext_Interop::GGetType),
			ZSHARP_BUILD_MANAGED_FUNCTION(FZMasterAssemblyLoadContext_Interop::GZCall_Red),
			ZSHARP_BUILD_MANAGED_FUNCTION(FZMasterAssemblyLoadContext_Interop::GGetZCallHandle_Red),
			ZSHARP_BUILD_MANAGED_FUNCTION(FZMasterAssemblyLoadContext_Interop::GBuildConjugate_Red),
			ZSHARP_BUILD_MANAGED_FUNCTION(FZMasterAssemblyLoadContext_Interop::GReleaseConjugate_Red),

			ZSHARP_BUILD_MANAGED_FUNCTION(FZSlimAssemblyLoadContext_Interop::GUnload),
			ZSHARP_BUILD_MANAGED_FUNCTION(FZSlimAssemblyLoadContext_Interop::GLoadAssembly),
			ZSHARP_BUILD_MANAGED_FUNCTION(FZSlimAssemblyLoadContext_Interop::GInvokeMethod),

			ZSHARP_BUILD_MANAGED_FUNCTION(FZConsole_Interop::GHandleExecuteCommand),
			ZSHARP_BUILD_MANAGED_FUNCTION(FZConsole_Interop::GHandleVariableChanged),
		};

		static const struct
		{
			void* IsInGameThreadFuncPtr = &IsInGameThread;
		} GUnmanagedProperties;
		
		static const struct
		{
			FZUnmanagedFunctions UnmanagedFunctions { UE_ARRAY_COUNT(GUnmanagedFunctions), GUnmanagedFunctions };
			void*** ManagedFunctions = GManagedFunctions;
			decltype(GUnmanagedProperties) UnmanagedProperties = GUnmanagedProperties;
		} GArgs{};

		void(*dllMain)(const decltype(GArgs)&) = nullptr;

		const FString assemblyName = ZSHARP_CORE_ASSEMBLY_NAME;
		const FString assemblyPath = FPaths::Combine(FPaths::ProjectDir(), "Binaries", "Managed", "Core", assemblyName + ".dll");
		const FString entryTypeName = FString::Printf(TEXT("%s.DllEntry, %s"), *assemblyName, *assemblyName);
		const FString entryMethodName = TEXT("DllMain");

		TArray<uint8> content;
		verify(FFileHelper::LoadFileToArray(content, *assemblyPath, FILEREAD_Silent));
		loadAssembly(content.GetData(), content.Num(), nullptr, 0, nullptr, nullptr);
		getFunctionPointer(*entryTypeName, *entryMethodName, UNMANAGEDCALLERSONLY_METHOD, nullptr, nullptr, reinterpret_cast<void**>(&dllMain));
		
		check(dllMain);
		dllMain(GArgs);
	}

	static void LoadResolverAssembly(load_assembly_bytes_fn loadAssembly)
	{
		const FString assemblyName = ZSHARP_RESOLVER_ASSEMBLY_NAME;
		const FString assemblyPath = FPaths::Combine(FPaths::ProjectDir(), "Binaries", "Managed", "Core", assemblyName + ".dll");
		const FString entryTypeName = FString::Printf(TEXT("%s.DllEntry, %s"), *assemblyName, *assemblyName);
		const FString entryMethodName = TEXT("DllMain");

		TArray<uint8> content;
		verify(FFileHelper::LoadFileToArray(content, *assemblyPath, FILEREAD_Silent));
		loadAssembly(content.GetData(), content.Num(), nullptr, 0, nullptr, nullptr);
	}

	static void LoadCoreEngineAssembly(load_assembly_bytes_fn loadAssembly, get_function_pointer_fn getFunctionPointer)
	{
		static FZUnmanagedFunction GUnmanagedFunctions[] =
		{
#define ZSHARP_UNMANAGED_FUNCTION_ASSEMBLY ZeroGames.ZSharp.Core.UnrealEngine
			
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Log, Log),
			
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Config, GetFileName),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Config, TryGetSection),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Config, TryGetArray),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Config, TryGetString),
			
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Console, TryExecuteCommand),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Console, TryGetValue),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Console, TrySetValue),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Console, TryRegisterOnChanged),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Console, TryUnregisterOnChanged),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Console, TryRegisterCommand),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Console, TryRegisterVariable),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Console, TryUnregisterObject),
			
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Build, WithEditor),
			
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Path, GetProjectDir),
			ZSHARP_BUILD_UNMANAGED_FUNCTION(Path, GetPluginDir),

#undef ZSHARP_UNMANAGED_FUNCTION_ASSEMBLY
		};

		static void** GManagedFunctions[] =
		{
			ZSHARP_BUILD_MANAGED_FUNCTION(FZConsole_Interop::GHandleExecuteCommand),
			ZSHARP_BUILD_MANAGED_FUNCTION(FZConsole_Interop::GHandleVariableChanged),
		};
		
		static const struct
		{
			void* IsInGameThreadFuncPtr = &IsInGameThread;
			bool* GIsServerPtr = &GIsServer;
			bool* GIsClientPtr = &GIsClient;
			bool* GIsEditorPtr = &GIsEditor;
			uint64* GFrameCounterPtr = &GFrameCounter;
			FConfigCacheIni* Config = GConfig;
		} GUnmanagedProperties;
		
		static const struct
        {
        	FZUnmanagedFunctions UnmanagedFunctions { UE_ARRAY_COUNT(GUnmanagedFunctions), GUnmanagedFunctions };
			void*** ManagedFunctions = GManagedFunctions;
			decltype(GUnmanagedProperties) UnmanagedProperties = GUnmanagedProperties;
        } GArgs{};

		void(*dllMain)(const decltype(GArgs)&) = nullptr;

		const FString assemblyName = ZSHARP_CORE_ENGINE_ASSEMBLY_NAME;
		const FString assemblyPath = FPaths::Combine(FPaths::ProjectDir(), "Binaries", "Managed", "Core", assemblyName + ".dll");
		const FString entryTypeName = FString::Printf(TEXT("%s.DllEntry, %s"), *assemblyName, *assemblyName);
		const FString entryMethodName = TEXT("DllMain");

		TArray<uint8> content;
		verify(FFileHelper::LoadFileToArray(content, *assemblyPath, FILEREAD_Silent));
		loadAssembly(content.GetData(), content.Num(), nullptr, 0, nullptr, nullptr);
		getFunctionPointer(*entryTypeName, *entryMethodName, UNMANAGEDCALLERSONLY_METHOD, nullptr, nullptr, reinterpret_cast<void**>(&dllMain));

		check(dllMain);
		dllMain(GArgs);
	}

	static void LoadCoreAsyncAssembly(load_assembly_bytes_fn loadAssembly, get_function_pointer_fn getFunctionPointer)
	{
		static void** managedFunctions[] =
		{
			ZSHARP_BUILD_MANAGED_FUNCTION(ZSharp::FZEventLoop_Interop::GNotifyEvent),
		};
		
		static const struct
		{
			void*** ManagedFunctions = managedFunctions;
		} GArgs{};

		void(*dllMain)(const decltype(GArgs)&) = nullptr;

		const FString assemblyName = ZSHARP_CORE_ASYNC_ASSEMBLY_NAME;
		const FString assemblyPath = FPaths::Combine(FPaths::ProjectDir(), "Binaries", "Managed", "Core", assemblyName + ".dll");
		const FString entryTypeName = FString::Printf(TEXT("%s.DllEntry, %s"), *assemblyName, *assemblyName);
		const FString entryMethodName = TEXT("DllMain");

		TArray<uint8> content;
		verify(FFileHelper::LoadFileToArray(content, *assemblyPath, FILEREAD_Silent));
		loadAssembly(content.GetData(), content.Num(), nullptr, 0, nullptr, nullptr);
		getFunctionPointer(*entryTypeName, *entryMethodName, UNMANAGEDCALLERSONLY_METHOD, nullptr, nullptr, reinterpret_cast<void**>(&dllMain));
		
		check(dllMain);
		dllMain(GArgs);
	}
}

ZSharp::FZGenericClr& ZSharp::FZGenericClr::Get()
{
	static FZGenericClr GInstance;

	return GInstance;
}

void ZSharp::FZGenericClr::Startup()
{
	check(IsInGameThread());
	
	if (bInitialized)
	{
		return;
	}

	bInitialized = true;

	const FString dotnetRoot = FPaths::Combine(FPaths::ProjectDir(), ZSHARP_DOTNET_PATH_TO_PROJECT);

	const FString hostFxrPath = FPaths::Combine(dotnetRoot, ZSHARP_HOSTFXR_PATH_TO_DOTNET);
	void* hostFxr = FPlatformProcess::GetDllHandle(*hostFxrPath);
	check(hostFxr);
	
	hostfxr_set_error_writer_fn setErrorWriter = (hostfxr_set_error_writer_fn)FPlatformProcess::GetDllExport(hostFxr, TEXT("hostfxr_set_error_writer"));
	check(setErrorWriter);
	setErrorWriter(&ZGenericClr_Private::HandleError);

	hostfxr_initialize_for_runtime_config_fn initializeHostFxr = (hostfxr_initialize_for_runtime_config_fn)FPlatformProcess::GetDllExport(hostFxr, TEXT("hostfxr_initialize_for_runtime_config"));
	check(initializeHostFxr);
	
	hostfxr_get_runtime_delegate_fn getRuntimeDelegate = (hostfxr_get_runtime_delegate_fn)FPlatformProcess::GetDllExport(hostFxr, TEXT("hostfxr_get_runtime_delegate"));
	check(getRuntimeDelegate);
	
	hostfxr_close_fn closeHostFXR = (hostfxr_close_fn)FPlatformProcess::GetDllExport(hostFxr, TEXT("hostfxr_close"));
	check(closeHostFXR);

	hostfxr_handle handle = nullptr;

	const FString projectConfigDir = FPaths::ProjectConfigDir();
	const FString runtimeConfigPath = FPaths::Combine(projectConfigDir, ZSHARP_RUNTIME_CONFIG_FILE_NAME);
	check(FPaths::FileExists(runtimeConfigPath));

	UE_LOG(LogZSharpCore, Log, TEXT("Runtime config file detected: [%s]."), *FPaths::ConvertRelativePathToFull(runtimeConfigPath));
	initializeHostFxr(*runtimeConfigPath, nullptr, &handle);
	check(handle);

	load_assembly_bytes_fn loadAssembly;
	getRuntimeDelegate(handle, hdt_load_assembly_bytes, reinterpret_cast<void**>(&loadAssembly));
	check(loadAssembly);

	get_function_pointer_fn getFunctionPointer;
	getRuntimeDelegate(handle, hdt_get_function_pointer, reinterpret_cast<void**>(&getFunctionPointer));

	closeHostFXR(handle);

	ZGenericClr_Private::LoadAssembliesUnderDirectory("ForwardShared", loadAssembly);
	ZGenericClr_Private::LoadCoreAssembly(loadAssembly, getFunctionPointer);
	ZGenericClr_Private::LoadResolverAssembly(loadAssembly);
	ZGenericClr_Private::LoadCoreEngineAssembly(loadAssembly, getFunctionPointer);
	ZGenericClr_Private::LoadCoreAsyncAssembly(loadAssembly, getFunctionPointer);
	ZGenericClr_Private::LoadAssembliesUnderDirectory("DeferredShared", loadAssembly);
	
	FCoreUObjectDelegates::GarbageCollectComplete.AddRaw(this, &ThisClass::HandleGarbageCollectComplete);
}

void ZSharp::FZGenericClr::Shutdown()
{
	check(IsInGameThread());
	
	if (!bInitialized)
	{
		return;
	}
	
	FCoreUObjectDelegates::GarbageCollectComplete.RemoveAll(this);

	if (MasterAlc)
	{
		MasterAlc->Unload();
	}

	for (const auto& Pair : SlimAlcMap)
	{
		Pair.Value->Unload();
	}

	bInitialized = false;
}

void ZSharp::FZGenericClr::CollectGarbage(int32 generation, bool aggressive, bool blocking, bool compacting)
{
	FZClr_Interop::GCollectGarbage(generation, aggressive, blocking, compacting);
}

ZSharp::IZDefaultAssemblyLoadContext& ZSharp::FZGenericClr::GetDefaultAlc()
{
	if (!DefaultAlc)
	{
		DefaultAlc = MakeUnique<FZDefaultAssemblyLoadContext>();
	}

	return *DefaultAlc;
}

ZSharp::IZMasterAssemblyLoadContext* ZSharp::FZGenericClr::CreateMasterAlc()
{
	check(IsInGameThread());
	
	if (MasterAlc)
	{
		UE_LOG(LogZSharpCore, Warning, TEXT("Master ALC already exists!"));
		return MasterAlc.Get();
	}

	if (!FZClr_Interop::GCreateMasterAlc())
	{
		UE_LOG(LogZSharpCore, Warning, TEXT("Master ALC already exists!"));
		return nullptr;
	}
	
	MasterAlc = MakeUnique<FZMasterAssemblyLoadContext>([this]{ HandleMasterAlcUnloaded(); });

	PreMasterAlcStartupDelegate.Broadcast(MasterAlc.Get());
	OnMasterAlcStartupDelegate.Broadcast(MasterAlc.Get());
	PostMasterAlcStartupDelegate.Broadcast(MasterAlc.Get());
	OnMasterAlcLoadedDelegate.Broadcast(MasterAlc.Get());

	return MasterAlc.Get();
}

ZSharp::IZMasterAssemblyLoadContext* ZSharp::FZGenericClr::GetMasterAlc()
{
	check(IsInGameThread());
	
	return MasterAlc.Get();
}

ZSharp::IZSlimAssemblyLoadContext* ZSharp::FZGenericClr::CreateSlimAlc(const FString& name)
{
	FWriteScopeLock _(SlimAlcMapLock);
	
	if (const auto pAlc = SlimAlcMap.Find(name))
	{
		UE_LOG(LogZSharpCore, Warning, TEXT("Slim ALC [%s] already exists!"), *name);
		return pAlc->Get();
	}

	FZGCHandle handle = FZClr_Interop::GCreateSlimAlc(*name);
	if (!handle)
	{
		return nullptr;
	}

	auto alc = new FZSlimAssemblyLoadContext { handle, [this, name]{ HandleSlimAlcUnloaded(name); }, name };
	SlimAlcMap.Emplace(name, alc);
	
	return alc;
}

ZSharp::IZSlimAssemblyLoadContext* ZSharp::FZGenericClr::GetSlimAlc(const FString& name)
{
	FReadScopeLock _(SlimAlcMapLock);

	
	const TUniquePtr<FZSlimAssemblyLoadContext>* pAlc = SlimAlcMap.Find(name);
	return pAlc ? pAlc->Get() : nullptr;
}

void ZSharp::FZGenericClr::HandleMasterAlcUnloaded()
{
	OnMasterAlcUnloadedDelegate.Broadcast();
	
	MasterAlc = nullptr;
}

void ZSharp::FZGenericClr::HandleSlimAlcUnloaded(const FString& name)
{
	FWriteScopeLock _(SlimAlcMapLock);
	
	SlimAlcMap.Remove(name);
}

FDelegateHandle ZSharp::FZGenericClr::CallOrRegisterOnMasterAlcLoaded(FZOnMasterAlcLoaded::FDelegate delegate)
{
	if (MasterAlc)
	{
		delegate.ExecuteIfBound(MasterAlc.Get());
	}

	// Register even if master ALC exists.
	return OnMasterAlcLoadedDelegate.Add(delegate);
}

void ZSharp::FZGenericClr::HandleGarbageCollectComplete()
{
	if (ZGenericClr_Private::GCVarPerformManagedGCAfterUnmanagedGC.GetValueOnGameThread())
	{
		CollectGarbage();
	}
}


