// Copyright Zero Games. All Rights Reserved.


#include "CLR/ZGenericClr.h"

#include "hostfxr.h"
#include "coreclr_delegates.h"
#include "ZSharpCoreLogChannels.h"
#include "Interfaces/IPluginManager.h"
#include "ALC/IZSlimAssemblyLoadContext.h"
#include "Interop/ZLog_Interop.h"
#include "Interop/Engine/ZUnrealEngine_Interop.h"
#include "Interop/ZClr_Interop.h"
#include "Interop/ZGCHandle_Interop.h"
#include "Interop/ZInteropString_Interop.h"
#include "ALC/ZMasterAssemblyLoadContext.h"
#include "Interop/ZMasterAssemblyLoadContext_Interop.h"
#include "Interop/ZGCHandle.h"
#include "ALC/ZSlimAssemblyLoadContext.h"
#include "Interop/ZSlimAssemblyLoadContext_Interop.h"
#include "Interop/Engine/ZBuild_Interop.h"
#include "Interop/Engine/ZPath_Interop.h"

namespace ZSharp::ZGenericClr_Private
{
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
		static const TCHAR* GLog_InteropTypeName = TEXT("ZeroGames.ZSharp.Core.Log_Interop");
		static const TCHAR* GInteropString_InteropTypeName = TEXT("ZeroGames.ZSharp.Core.InteropString_Interop");
		static const TCHAR* GMasterAssemblyLoadContext_InteropTypeName = TEXT("ZeroGames.ZSharp.Core.MasterAssemblyLoadContext_Interop");
		
		static FZUnmanagedFunction GUnmanagedFunctions[] =
        {
#define BUILD_UNMANAGED_FUNCTION(ShortTypeName, FieldName) { G##ShortTypeName##TypeName, TEXT(#FieldName), FZ##ShortTypeName::FieldName }

			BUILD_UNMANAGED_FUNCTION(Log_Interop, Log),
			
			BUILD_UNMANAGED_FUNCTION(InteropString_Interop, Alloc),
			BUILD_UNMANAGED_FUNCTION(InteropString_Interop, Free),
			BUILD_UNMANAGED_FUNCTION(InteropString_Interop, GetData),
			BUILD_UNMANAGED_FUNCTION(InteropString_Interop, SetData),

			BUILD_UNMANAGED_FUNCTION(MasterAssemblyLoadContext_Interop, ZCall_Black),
			BUILD_UNMANAGED_FUNCTION(MasterAssemblyLoadContext_Interop, GetZCallHandle_Black),
			BUILD_UNMANAGED_FUNCTION(MasterAssemblyLoadContext_Interop, BuildConjugate_Black),
			BUILD_UNMANAGED_FUNCTION(MasterAssemblyLoadContext_Interop, ReleaseConjugate_Black),

#undef BUILD_UNMANAGED_FUNCTION
        };

		static void** GManagedFunctions[] =
		{
#define ADDRESS_OF(Pointer) reinterpret_cast<void**>(&Pointer)

			ADDRESS_OF(FZClr_Interop::GCollectGarbage),
			ADDRESS_OF(FZClr_Interop::GCreateMasterAlc),
			ADDRESS_OF(FZClr_Interop::GCreateSlimAlc),

			ADDRESS_OF(FZGCHandle_Interop::GFree),

			ADDRESS_OF(FZMasterAssemblyLoadContext_Interop::GTick),
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
				
#undef ADDRESS_OF
		};
		
		static const struct
		{
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

	static void LoadCoreEngineAssembly(load_assembly_bytes_fn loadAssembly, get_function_pointer_fn getFunctionPointer)
	{
		static const FString GBuildInteropTypeName = FString::Printf(TEXT("%s.%s"), TEXT(ZSHARP_CORE_ENGINE_ASSEMBLY_NAME), TEXT("Build_Interop"));
		static const FString GUnrealEngineInteropTypeName = FString::Printf(TEXT("%s.%s"), TEXT(ZSHARP_CORE_ENGINE_ASSEMBLY_NAME), TEXT("UnrealEngine_Interop"));
		static const FString GPathInteropTypeName = FString::Printf(TEXT("%s.%s"), TEXT(ZSHARP_CORE_ENGINE_ASSEMBLY_NAME), TEXT("Path_Interop"));;
		
		static FZUnmanagedFunction GUnmanagedFunctions[] =
		{
			{ *GBuildInteropTypeName, TEXT("WithEditor"), FZBuild_Interop::WithEditor },
		
			{ *GUnrealEngineInteropTypeName, TEXT("IsInGameThread"), FZUnrealEngine_Interop::IsInGameThread },

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
}

ZSharp::FZGenericClr& ZSharp::FZGenericClr::Get()
{
	static FZGenericClr GSingleton;

	return GSingleton;
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

	const FString hostFXRPath = FPaths::Combine(dotnetRoot, ZSHARP_HOSTFXR_PATH_TO_DOTNET);
	void* hostFXR = FPlatformProcess::GetDllHandle(*hostFXRPath);
	check(hostFXR);

	hostfxr_initialize_for_runtime_config_fn initializeHostFxr = (hostfxr_initialize_for_runtime_config_fn)
		FPlatformProcess::GetDllExport(hostFXR, TEXT("hostfxr_initialize_for_runtime_config"));
	check(initializeHostFxr);
	hostfxr_get_runtime_delegate_fn getRuntimeDelegate = (hostfxr_get_runtime_delegate_fn)
		FPlatformProcess::GetDllExport(hostFXR, TEXT("hostfxr_get_runtime_delegate"));
	check(getRuntimeDelegate);
	hostfxr_close_fn closeHostFXR = (hostfxr_close_fn)FPlatformProcess::GetDllExport(hostFXR, TEXT("hostfxr_close"));
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
	ZGenericClr_Private::LoadCoreEngineAssembly(loadAssembly, getFunctionPointer);
	ZGenericClr_Private::LoadResolverAssembly(loadAssembly);
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

ZSharp::IZMasterAssemblyLoadContext* ZSharp::FZGenericClr::CreateMasterAlc()
{
	check(IsInGameThread());
	
	if (MasterAlc)
	{
		UE_LOG(LogZSharpCore, Fatal, TEXT("Master ALC already exists!"));
	}

	FZGCHandle handle = FZClr_Interop::GCreateMasterAlc();
	if (!handle)
	{
		return nullptr;
	}
	
	MasterAlc = MakeUnique<FZMasterAssemblyLoadContext>(handle, [this]{ HandleMasterAlcUnloaded(); });

	OnMasterAlcLoaded.Broadcast(MasterAlc.Get());

	OnMasterAlcLoadFrameworks.Broadcast(MasterAlc.Get());
	OnMasterAlcLoadApplications.Broadcast(MasterAlc.Get());
	OnMasterAlcLoadPlugins.Broadcast(MasterAlc.Get());
	
	OnMasterAlcFullyLoaded.Broadcast(MasterAlc.Get());

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
	
	if (SlimAlcMap.Contains(name))
	{
		UE_LOG(LogZSharpCore, Fatal, TEXT("Slim ALC [%s] already exists!"), *name);
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
	OnMasterAlcUnloaded.Broadcast();
	
	MasterAlc = nullptr;
}

void ZSharp::FZGenericClr::HandleSlimAlcUnloaded(const FString& name)
{
	FWriteScopeLock _(SlimAlcMapLock);
	
	SlimAlcMap.Remove(name);
}

FDelegateHandle ZSharp::FZGenericClr::RegisterMasterAlcLoaded(FZOnMasterAlcLoaded::FDelegate delegate, bool notifyIfLoaded)
{
	check(IsInGameThread());

	FDelegateHandle handle = OnMasterAlcLoaded.Add(delegate);
	if (notifyIfLoaded && MasterAlc)
	{
		delegate.ExecuteIfBound(MasterAlc.Get());
	}

	return handle;
}

void ZSharp::FZGenericClr::UnregisterMasterAlcLoaded(FDelegateHandle delegate)
{
	check(IsInGameThread());
	
	OnMasterAlcLoaded.Remove(delegate);
}

void ZSharp::FZGenericClr::UnregisterMasterAlcLoaded(const void* userObject)
{
	check(IsInGameThread());
	
	OnMasterAlcLoaded.RemoveAll(userObject);
}

FDelegateHandle ZSharp::FZGenericClr::RegisterMasterAlcLoadFrameworks(FZOnMasterAlcLoadFrameworks::FDelegate delegate, bool notifyIfLoaded)
{
	check(IsInGameThread());

	FDelegateHandle handle = OnMasterAlcLoadFrameworks.Add(delegate);
	if (notifyIfLoaded && MasterAlc)
	{
		delegate.ExecuteIfBound(MasterAlc.Get());
	}

	return handle;
}

void ZSharp::FZGenericClr::UnregisterMasterAlcLoadFrameworks(FDelegateHandle delegate)
{
	check(IsInGameThread());
	
	OnMasterAlcLoadFrameworks.Remove(delegate);
}

void ZSharp::FZGenericClr::UnregisterMasterAlcLoadFrameworks(const void* userObject)
{
	check(IsInGameThread());
	
	OnMasterAlcLoadFrameworks.RemoveAll(userObject);
}

FDelegateHandle ZSharp::FZGenericClr::RegisterMasterAlcLoadApplications(FZOnMasterAlcLoadApplications::FDelegate delegate, bool notifyIfLoaded)
{
	check(IsInGameThread());

	FDelegateHandle handle = OnMasterAlcLoadApplications.Add(delegate);
	if (notifyIfLoaded && MasterAlc)
	{
		delegate.ExecuteIfBound(MasterAlc.Get());
	}

	return handle;
}

void ZSharp::FZGenericClr::UnregisterMasterAlcLoadApplications(FDelegateHandle delegate)
{
	check(IsInGameThread());
	
	OnMasterAlcLoadApplications.Remove(delegate);
}

void ZSharp::FZGenericClr::UnregisterMasterAlcLoadApplications(const void* userObject)
{
	check(IsInGameThread());
	
	OnMasterAlcLoadApplications.RemoveAll(userObject);
}

FDelegateHandle ZSharp::FZGenericClr::RegisterMasterAlcLoadPlugins(FZOnMasterAlcLoadPlugins::FDelegate delegate, bool notifyIfLoaded)
{
	check(IsInGameThread());

	FDelegateHandle handle = OnMasterAlcLoadPlugins.Add(delegate);
	if (notifyIfLoaded && MasterAlc)
	{
		delegate.ExecuteIfBound(MasterAlc.Get());
	}

	return handle;
}

void ZSharp::FZGenericClr::UnregisterMasterAlcLoadPlugins(FDelegateHandle delegate)
{
	check(IsInGameThread());
	
	OnMasterAlcLoadPlugins.Remove(delegate);
}

void ZSharp::FZGenericClr::UnregisterMasterAlcLoadPlugins(const void* userObject)
{
	check(IsInGameThread());
	
	OnMasterAlcLoadPlugins.RemoveAll(userObject);
}

FDelegateHandle ZSharp::FZGenericClr::RegisterMasterAlcFullyLoaded(FZOnMasterAlcFullyLoaded::FDelegate delegate, bool notifyIfLoaded)
{
	check(IsInGameThread());

	FDelegateHandle handle = OnMasterAlcFullyLoaded.Add(delegate);
	if (notifyIfLoaded && MasterAlc)
	{
		delegate.ExecuteIfBound(MasterAlc.Get());
	}

	return handle;
}

void ZSharp::FZGenericClr::UnregisterMasterAlcFullyLoaded(FDelegateHandle delegate)
{
	check(IsInGameThread());
	
	OnMasterAlcFullyLoaded.Remove(delegate);
}

void ZSharp::FZGenericClr::UnregisterMasterAlcFullyLoaded(const void* userObject)
{
	check(IsInGameThread());
	
	OnMasterAlcFullyLoaded.RemoveAll(userObject);
}

FDelegateHandle ZSharp::FZGenericClr::RegisterMasterAlcUnloaded(FZOnMasterAlcUnloaded::FDelegate delegate)
{
	check(IsInGameThread());
	
	return OnMasterAlcUnloaded.Add(delegate);
}

void ZSharp::FZGenericClr::UnregisterMasterAlcUnloaded(FDelegateHandle delegate)
{
	check(IsInGameThread());
	
	OnMasterAlcUnloaded.Remove(delegate);
}

void ZSharp::FZGenericClr::UnregisterMasterAlcUnloaded(const void* userObject)
{
	check(IsInGameThread());
	
	OnMasterAlcUnloaded.RemoveAll(userObject);
}

void ZSharp::FZGenericClr::HandleGarbageCollectComplete()
{
	CollectGarbage();
}


