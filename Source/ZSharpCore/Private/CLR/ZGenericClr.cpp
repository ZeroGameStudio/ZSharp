// Copyright Zero Games. All Rights Reserved.


#include "CLR/ZGenericClr.h"

#include "hostfxr.h"
#include "coreclr_delegates.h"
#include "ZSharpCoreLogChannels.h"
#include "Interfaces/IPluginManager.h"
#include "ALC/IZSlimAssemblyLoadContext.h"
#include "Interop/Core_Interop.h"
#include "Interop/UnrealEngine_Interop.h"
#include "Interop/ZClr_Interop.h"
#include "Interop/ZGCHandle_Interop.h"
#include "Interop/ZInteropString_Interop.h"
#include "ALC/ZMasterAssemblyLoadContext.h"
#include "Interop/ZMasterAssemblyLoadContext_Interop.h"
#include "Interop/ZGCHandle.h"
#include "ALC/ZSlimAssemblyLoadContext.h"
#include "Interop/ZSlimAssemblyLoadContext_Interop.h"

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
	
	static void LoadCoreAssembly(const FString& pluginContentDir, load_assembly_bytes_fn loadAssembly, get_function_pointer_fn getFunctionPointer)
	{
		const TCHAR* Core_InteropTypeName = TEXT("ZeroGames.ZSharp.Core.Core_Interop");
		const TCHAR* InteropString_InteropTypeName = TEXT("ZeroGames.ZSharp.Core.InteropString_Interop");
		const TCHAR* MasterAssemblyLoadContext_InteropTypeName = TEXT("ZeroGames.ZSharp.Core.MasterAssemblyLoadContext_Interop");
		
		FZUnmanagedFunction unmanagedFunctions[] =
        {
#define TO_STRING(FieldName) TEXT(#FieldName)
#define BUILD_UNMANAGED_FUNCTION(ShortTypeName, FieldName) { ShortTypeName##TypeName, TO_STRING(S##FieldName), FZ##ShortTypeName::FieldName }

			BUILD_UNMANAGED_FUNCTION(Core_Interop, CoreLog),
			
			BUILD_UNMANAGED_FUNCTION(InteropString_Interop, Alloc),
			BUILD_UNMANAGED_FUNCTION(InteropString_Interop, Free),
			BUILD_UNMANAGED_FUNCTION(InteropString_Interop, GetData),
			BUILD_UNMANAGED_FUNCTION(InteropString_Interop, SetData),

			BUILD_UNMANAGED_FUNCTION(MasterAssemblyLoadContext_Interop, ZCall_Black),
			BUILD_UNMANAGED_FUNCTION(MasterAssemblyLoadContext_Interop, GetZCallHandle_Black),
			BUILD_UNMANAGED_FUNCTION(MasterAssemblyLoadContext_Interop, BuildConjugate_Black),
			BUILD_UNMANAGED_FUNCTION(MasterAssemblyLoadContext_Interop, ReleaseConjugate_Black),

#undef BUILD_UNMANAGED_FUNCTION
#undef TO_STRING
        };

		void** managedFunctions[] =
		{
#define ADDRESS_OF(Pointer) reinterpret_cast<void**>(&Pointer)

			ADDRESS_OF(FZClr_Interop::GCollectGarbage),
			ADDRESS_OF(FZClr_Interop::GCreateMasterAlc),
			ADDRESS_OF(FZClr_Interop::GCreateSlimAlc),

			ADDRESS_OF(FZGCHandle_Interop::GFree),

			ADDRESS_OF(FZMasterAssemblyLoadContext_Interop::GTick),
			ADDRESS_OF(FZMasterAssemblyLoadContext_Interop::GUnload),
			ADDRESS_OF(FZMasterAssemblyLoadContext_Interop::GLoadAssembly),
			ADDRESS_OF(FZMasterAssemblyLoadContext_Interop::GGetType),
			ADDRESS_OF(FZMasterAssemblyLoadContext_Interop::GZCall_Red),
			ADDRESS_OF(FZMasterAssemblyLoadContext_Interop::GGetZCallHandle_Red),
			ADDRESS_OF(FZMasterAssemblyLoadContext_Interop::GBuildConjugate_Red),
			ADDRESS_OF(FZMasterAssemblyLoadContext_Interop::GReleaseConjugate_Red),

			ADDRESS_OF(FZSlimAssemblyLoadContext_Interop::GUnload),
			ADDRESS_OF(FZSlimAssemblyLoadContext_Interop::GLoadAssembly),
			ADDRESS_OF(FZSlimAssemblyLoadContext_Interop::GCallMethod),
				
#undef ADDRESS_OF
		};
		
		struct
		{
			FZUnmanagedFunctions UnmanagedFunctions { UE_ARRAY_COUNT(unmanagedFunctions), unmanagedFunctions };
			void*** ManagedFunctions = managedFunctions;
		} args{};

		int32(*dllMain)(const decltype(args)&) = nullptr;

		const FString assemblyName = ZSHARP_CORE_ASSEMBLY_NAME;
		FString assemblyPath = FPaths::Combine(FPaths::ProjectDir(), "Binaries/Managed", assemblyName + ".dll");
		const FString entryTypeName = FString::Printf(TEXT("%s.DllEntry, %s"), *assemblyName, *assemblyName);
		const FString entryMethodName = TEXT("DllMain");

		TArray<uint8> content;
		if (!FFileHelper::LoadFileToArray(content, *assemblyPath, FILEREAD_Silent))
		{
			assemblyPath = FPaths::Combine(pluginContentDir, assemblyName + ".dll");
			if (!FFileHelper::LoadFileToArray(content, *assemblyPath, FILEREAD_Silent))
			{
				checkNoEntry();
			}
		}
		loadAssembly(content.GetData(), content.Num(), nullptr, 0, nullptr, nullptr);
		getFunctionPointer(*entryTypeName, *entryMethodName, UNMANAGEDCALLERSONLY_METHOD, nullptr, nullptr, reinterpret_cast<void**>(&dllMain));
		
		check(dllMain);
		dllMain(args);
	}

	static void LoadEngineCoreAssembly(const FString& pluginContentDir, load_assembly_bytes_fn loadAssembly, get_function_pointer_fn getFunctionPointer)
	{
		FString typeName = FString::Printf(TEXT("%s.%s"), TEXT(ZSHARP_CORE_ENGINE_ASSEMBLY_NAME), TEXT("UnrealEngine_Interop"));

		FZUnmanagedFunction unmanagedFunctions[] =
		{
			{ *typeName, TEXT("SLog"), FZUnrealEngine_Interop::Log },
			{ *typeName, TEXT("SIsInGameThread"), FZUnrealEngine_Interop::IsInGameThread },
		};
		
		struct
        {
        	FZUnmanagedFunctions UnmanagedFunctions { UE_ARRAY_COUNT(unmanagedFunctions), unmanagedFunctions };
        } args{};

		int32(*dllMain)(const decltype(args)&) = nullptr;

		const FString assemblyName = ZSHARP_CORE_ENGINE_ASSEMBLY_NAME;
		FString assemblyPath = FPaths::Combine(FPaths::ProjectDir(), "Binaries/Managed", assemblyName + ".dll");
		const FString entryTypeName = FString::Printf(TEXT("%s.DllEntry, %s"), *assemblyName, *assemblyName);
		const FString entryMethodName = TEXT("DllMain");

		TArray<uint8> content;
		if (!FFileHelper::LoadFileToArray(content, *assemblyPath, FILEREAD_Silent))
		{
			assemblyPath = FPaths::Combine(pluginContentDir, assemblyName + ".dll");
			if (!FFileHelper::LoadFileToArray(content, *assemblyPath, FILEREAD_Silent))
			{
				checkNoEntry();
			}
		}
		loadAssembly(content.GetData(), content.Num(), nullptr, 0, nullptr, nullptr);
		getFunctionPointer(*entryTypeName, *entryMethodName, UNMANAGEDCALLERSONLY_METHOD, nullptr, nullptr, reinterpret_cast<void**>(&dllMain));

		check(dllMain);
		dllMain(args);
	}
}

void ZSharp::FZGenericClr::Startup()
{
	check(IsInGameThread());
	
	if (bInitialized)
	{
		return;
	}

	bInitialized = true;

	const FString pluginDir = IPluginManager::Get().FindEnabledPlugin("ZSharp")->GetBaseDir();
	const FString pluginBinariesDir = FPaths::Combine(pluginDir, "Binaries");
	const FString pluginContentDir = FPaths::Combine(pluginDir, "Content");
	
	const FString dotnetRoot = FPaths::Combine(pluginBinariesDir, ZSHARP_DOTNET_PATH);
	const FString hostFXRPath = FPaths::Combine(dotnetRoot, ZSHARP_HOSTFXR_PATH);
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
	const FString runtimeConfigPath = FPaths::Combine(pluginContentDir, ZSHARP_RUNTIME_CONFIG_FILE_NAME);
	initializeHostFxr(*runtimeConfigPath, nullptr, &handle);
	check(handle);

	load_assembly_bytes_fn loadAssembly;
	getRuntimeDelegate(handle, hdt_load_assembly_bytes, reinterpret_cast<void**>(&loadAssembly));
	check(loadAssembly);

	get_function_pointer_fn getFunctionPointer;
	getRuntimeDelegate(handle, hdt_get_function_pointer, reinterpret_cast<void**>(&getFunctionPointer));

	closeHostFXR(handle);

	ZGenericClr_Private::LoadCoreAssembly(pluginContentDir, loadAssembly, getFunctionPointer);
	ZGenericClr_Private::LoadEngineCoreAssembly(pluginContentDir, loadAssembly, getFunctionPointer);
	
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
	
	const TUniquePtr<IZSlimAssemblyLoadContext>* pAlc = SlimAlcMap.Find(name);
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


