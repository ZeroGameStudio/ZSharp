// Copyright Zero Games. All Rights Reserved.

#include "CLR/ZGenericClr.h"

#include "hostfxr.h"
#include "coreclr_delegates.h"
#include "ZSharpClr.h"
#include "ZSharpCoreLogChannels.h"
#include "ALC/IZSlimAssemblyLoadContext.h"
#include "Interop/Misc/Console_Interop.h"
#include "Interop/Misc/ZLog_Interop.h"
#include "Interop/Core/ZClr_Interop.h"
#include "Interop/Core/ZGCHandle_Interop.h"
#include "Interop/Misc/ZInteropString_Interop.h"
#include "Interop/Misc/ZInteropStringArray_Interop.h"
#include "ALC/ZMasterAssemblyLoadContext.h"
#include "Interop/Core/ZMasterAssemblyLoadContext_Interop.h"
#include "Interop/ZGCHandle.h"
#include "ALC/ZSlimAssemblyLoadContext.h"
#include "Interop/Misc/ZConfig_Interop.h"
#include "Interop/Core/ZDefaultAssemblyLoadContext_Interop.h"
#include "Interop/Async/ZEventLoop_Interop.h"
#include "Interop/Core/ZSlimAssemblyLoadContext_Interop.h"
#include "Interop/Engine/ZBuild_Interop.h"
#include "Interop/Engine/ZPath_Interop.h"

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
	
	struct FZUnmanagedFunction
	{
		const TCHAR* TypeName;
		const TCHAR* FieldName;
		void* Address;
	};
	
	struct FZUnmanagedFunctions
	{
		int32 Count;
		FZUnmanagedFunction* Functions;
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
		static const TCHAR* GInteropString_InteropTypeName = TEXT("ZeroGames.ZSharp.Core.InteropString_Interop");
		static const TCHAR* GInteropStringArray_InteropTypeName = TEXT("ZeroGames.ZSharp.Core.InteropStringArray_Interop");
		static const TCHAR* GMasterAssemblyLoadContext_InteropTypeName = TEXT("ZeroGames.ZSharp.Core.MasterAssemblyLoadContext_Interop");
		static const TCHAR* GLog_InteropTypeName = TEXT("ZeroGames.ZSharp.Core.Log_Interop");
		static const TCHAR* GConfig_InteropTypeName = TEXT("ZeroGames.ZSharp.Core.Config_Interop");
		static const TCHAR* GConsole_InteropTypeName = TEXT("ZeroGames.ZSharp.Core.Console_Interop");
		
		static FZUnmanagedFunction GUnmanagedFunctions[] =
        {
#define BUILD_UNMANAGED_FUNCTION(ShortTypeName, FieldName) { G##ShortTypeName##TypeName, TEXT(#FieldName), FZ##ShortTypeName::FieldName }
			
			BUILD_UNMANAGED_FUNCTION(InteropString_Interop, Alloc),
			BUILD_UNMANAGED_FUNCTION(InteropString_Interop, Free),
			BUILD_UNMANAGED_FUNCTION(InteropString_Interop, GetData),
			BUILD_UNMANAGED_FUNCTION(InteropString_Interop, SetData),

			BUILD_UNMANAGED_FUNCTION(InteropStringArray_Interop, Alloc),
			BUILD_UNMANAGED_FUNCTION(InteropStringArray_Interop, Free),
			BUILD_UNMANAGED_FUNCTION(InteropStringArray_Interop, Count),
			BUILD_UNMANAGED_FUNCTION(InteropStringArray_Interop, Get),
			BUILD_UNMANAGED_FUNCTION(InteropStringArray_Interop, Set),
			BUILD_UNMANAGED_FUNCTION(InteropStringArray_Interop, Add),
			BUILD_UNMANAGED_FUNCTION(InteropStringArray_Interop, InsertAt),
			BUILD_UNMANAGED_FUNCTION(InteropStringArray_Interop, RemoveAt),
			BUILD_UNMANAGED_FUNCTION(InteropStringArray_Interop, Clear),

			BUILD_UNMANAGED_FUNCTION(MasterAssemblyLoadContext_Interop, ZCall_Black),
			BUILD_UNMANAGED_FUNCTION(MasterAssemblyLoadContext_Interop, GetZCallHandle_Black),
			BUILD_UNMANAGED_FUNCTION(MasterAssemblyLoadContext_Interop, BuildConjugate_Black),
			BUILD_UNMANAGED_FUNCTION(MasterAssemblyLoadContext_Interop, ReleaseConjugate_Black),

			BUILD_UNMANAGED_FUNCTION(Log_Interop, Log),

			BUILD_UNMANAGED_FUNCTION(Config_Interop, GetFileName),
			BUILD_UNMANAGED_FUNCTION(Config_Interop, TryGetSection),
			BUILD_UNMANAGED_FUNCTION(Config_Interop, TryGetArray),
			BUILD_UNMANAGED_FUNCTION(Config_Interop, TryGetString),

			BUILD_UNMANAGED_FUNCTION(Console_Interop, TryExecuteCommand),
			BUILD_UNMANAGED_FUNCTION(Console_Interop, TryGetValue),
			BUILD_UNMANAGED_FUNCTION(Console_Interop, TrySetValue),
			BUILD_UNMANAGED_FUNCTION(Console_Interop, TryRegisterOnChanged),
			BUILD_UNMANAGED_FUNCTION(Console_Interop, TryUnregisterOnChanged),
			BUILD_UNMANAGED_FUNCTION(Console_Interop, TryRegisterCommand),
			BUILD_UNMANAGED_FUNCTION(Console_Interop, TryRegisterVariable),
			BUILD_UNMANAGED_FUNCTION(Console_Interop, TryUnregisterObject),

#undef BUILD_UNMANAGED_FUNCTION
        };

		static void** GManagedFunctions[] =
		{
#define ADDRESS_OF(Pointer) reinterpret_cast<void**>(&Pointer)

			ADDRESS_OF(FZClr_Interop::GCollectGarbage),
			ADDRESS_OF(FZClr_Interop::GCreateMasterAlc),
			ADDRESS_OF(FZClr_Interop::GCreateSlimAlc),

			ADDRESS_OF(FZGCHandle_Interop::GFree),

			ADDRESS_OF(FZDefaultAssemblyLoadContext_Interop::GLoadAssembly),
			ADDRESS_OF(FZDefaultAssemblyLoadContext_Interop::GInvokeMethod),

			ADDRESS_OF(FZMasterAssemblyLoadContext_Interop::GUnload),
			ADDRESS_OF(FZMasterAssemblyLoadContext_Interop::GLoadAssembly),
			ADDRESS_OF(FZMasterAssemblyLoadContext_Interop::GInvokeMethod),
			ADDRESS_OF(FZMasterAssemblyLoadContext_Interop::GGetType),
			ADDRESS_OF(FZMasterAssemblyLoadContext_Interop::GZCall_Red),
			ADDRESS_OF(FZMasterAssemblyLoadContext_Interop::GGetZCallHandle_Red),
			ADDRESS_OF(FZMasterAssemblyLoadContext_Interop::GBuildConjugate_Red),
			ADDRESS_OF(FZMasterAssemblyLoadContext_Interop::GReleaseConjugate_Red),

			ADDRESS_OF(FZSlimAssemblyLoadContext_Interop::GUnload),
			ADDRESS_OF(FZSlimAssemblyLoadContext_Interop::GLoadAssembly),
			ADDRESS_OF(FZSlimAssemblyLoadContext_Interop::GInvokeMethod),

			ADDRESS_OF(FZConsole_Interop::GHandleExecuteCommand),
			ADDRESS_OF(FZConsole_Interop::GHandleVariableChanged),
				
#undef ADDRESS_OF
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
			decltype(GUnmanagedProperties) UnmanagedProperties = GUnmanagedProperties;
			FZUnmanagedFunctions UnmanagedFunctions { UE_ARRAY_COUNT(GUnmanagedFunctions), GUnmanagedFunctions };
			void*** ManagedFunctions = GManagedFunctions;
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

	static void LoadCoreAsyncAssembly(load_assembly_bytes_fn loadAssembly, get_function_pointer_fn getFunctionPointer)
	{
		static void** managedFunctions[] =
		{
#define ADDRESS_OF(Pointer) reinterpret_cast<void**>(&Pointer)

			ADDRESS_OF(ZSharp::FZEventLoop_Interop::GNotifyEvent),
				
	#undef ADDRESS_OF
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

	static void LoadCoreEngineAssembly(load_assembly_bytes_fn loadAssembly, get_function_pointer_fn getFunctionPointer)
	{
		static const FString GBuildInteropTypeName = FString::Printf(TEXT("%s.%s"), TEXT(ZSHARP_CORE_ENGINE_ASSEMBLY_NAME), TEXT("Build_Interop"));
		static const FString GPathInteropTypeName = FString::Printf(TEXT("%s.%s"), TEXT(ZSHARP_CORE_ENGINE_ASSEMBLY_NAME), TEXT("Path_Interop"));;
		
		static FZUnmanagedFunction GUnmanagedFunctions[] =
		{
			{ *GBuildInteropTypeName, TEXT("WithEditor"), FZBuild_Interop::WithEditor },
		
			{ *GPathInteropTypeName, TEXT("GetProjectDir"), FZPath_Interop::GetProjectDir },
			{ *GPathInteropTypeName, TEXT("GetPluginDir"), FZPath_Interop::GetPluginDir },
		};
		
		static const struct
        {
        	FZUnmanagedFunctions UnmanagedFunctions { UE_ARRAY_COUNT(GUnmanagedFunctions), GUnmanagedFunctions };
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
	ZGenericClr_Private::LoadCoreAsyncAssembly(loadAssembly, getFunctionPointer);
	ZGenericClr_Private::LoadCoreEngineAssembly(loadAssembly, getFunctionPointer);
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

	FZGCHandle handle = FZClr_Interop::GCreateMasterAlc();
	if (!handle)
	{
		return nullptr;
	}
	
	MasterAlc = MakeUnique<FZMasterAssemblyLoadContext>(handle, [this]{ HandleMasterAlcUnloaded(); });

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


