// Copyright Zero Games. All Rights Reserved.


#include "CLR/ZGenericCLR.h"

#include "hostfxr.h"
#include "coreclr_delegates.h"
#include "ZSharpCLRLogChannels.h"
#include "Interfaces/IPluginManager.h"
#include "ALC/IZSlimAssemblyLoadContext.h"
#include "Interop/Core_Interop.h"
#include "Interop/UnrealEngine_Interop.h"
#include "Interop/ZAssembly_Interop.h"
#include "Interop/ZCLR_Interop.h"
#include "Interop/ZGCHandle_Interop.h"
#include "Interop/ZInteropString_Interop.h"
#include "ALC/ZMasterAssemblyLoadContext.h"
#include "Interop/ZMasterAssemblyLoadContext_Interop.h"
#include "Interop/ZGCHandle.h"
#include "Interop/ZMethodInfo_Interop.h"
#include "ALC/ZSlimAssemblyLoadContext.h"
#include "Interop/ZSlimAssemblyLoadContext_Interop.h"
#include "Interop/ZType_Interop.h"

namespace ZSharp::FZGenericCLR_Private
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

			BUILD_UNMANAGED_FUNCTION(MasterAssemblyLoadContext_Interop, ZCallByHandle),
			BUILD_UNMANAGED_FUNCTION(MasterAssemblyLoadContext_Interop, ZCallByName),
			BUILD_UNMANAGED_FUNCTION(MasterAssemblyLoadContext_Interop, ZCallByHandle_AnyThread),
			BUILD_UNMANAGED_FUNCTION(MasterAssemblyLoadContext_Interop, GetZCallHandle),

#undef BUILD_UNMANAGED_FUNCTION
#undef TO_STRING
        };

		void** managedFunctions[] =
		{
#define ADDRESS_OF(Pointer) reinterpret_cast<void**>(&Pointer)

			ADDRESS_OF(ZCLR_Interop::GCollectGarbage),
			ADDRESS_OF(ZCLR_Interop::GCreateMasterALC),
			ADDRESS_OF(ZCLR_Interop::GCreateSlimALC),

			ADDRESS_OF(FZGCHandle_Interop::GFree),

			ADDRESS_OF(FZMasterAssemblyLoadContext_Interop::GUnload),
			ADDRESS_OF(FZMasterAssemblyLoadContext_Interop::GLoadAssembly),
			ADDRESS_OF(FZMasterAssemblyLoadContext_Interop::GReleaseConjugate),

			ADDRESS_OF(FZSlimAssemblyLoadContext_Interop::GUnload),
			ADDRESS_OF(FZSlimAssemblyLoadContext_Interop::GLoadAssembly),
			ADDRESS_OF(FZSlimAssemblyLoadContext_Interop::GCallMethod),
				
			ADDRESS_OF(FZAssembly_Interop::GGetName),
			ADDRESS_OF(FZAssembly_Interop::GGetType),

			ADDRESS_OF(FZType_Interop::GGetName),
			ADDRESS_OF(FZType_Interop::GBuildConjugate),
			ADDRESS_OF(FZType_Interop::GGetMethodInfo),
			ADDRESS_OF(FZType_Interop::GGetPropertyInfo),

			ADDRESS_OF(FZMethodInfo_Interop::GGetName),
			ADDRESS_OF(FZMethodInfo_Interop::GGetNumSlots),
			ADDRESS_OF(FZMethodInfo_Interop::GInvoke),
				
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
		const TCHAR* typeName = TEXT("ZeroGames.ZSharp.UnrealEngine.Core.UnrealEngine_Interop");

		FZUnmanagedFunction unmanagedFunctions[] =
		{
			{ typeName, TEXT("SLog"), FZUnrealEngine_Interop::Log },
			{ typeName, TEXT("SIsInGameThread"), FZUnrealEngine_Interop::IsInGameThread },
		};
		
		struct
        {
        	FZUnmanagedFunctions UnmanagedFunctions { UE_ARRAY_COUNT(unmanagedFunctions), unmanagedFunctions };
        } args{};

		int32(*dllMain)(const decltype(args)&) = nullptr;

		const FString assemblyName = ZSHARP_ENGINE_CORE_ASSEMBLY_NAME;
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

void ZSharp::FZGenericCLR::Startup()
{
	check(IsInGameThread());
	
	if (bInitialized)
	{
		return;
	}

	bInitialized = true;

	const FString pluginDir = IPluginManager::Get().GetModuleOwnerPlugin(UE_MODULE_NAME)->GetBaseDir();
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

	FZGenericCLR_Private::LoadCoreAssembly(pluginContentDir, loadAssembly, getFunctionPointer);
	FZGenericCLR_Private::LoadEngineCoreAssembly(pluginContentDir, loadAssembly, getFunctionPointer);
	
	FCoreUObjectDelegates::GarbageCollectComplete.AddRaw(this, &ThisClass::HandleGarbageCollectComplete);
}

void ZSharp::FZGenericCLR::Shutdown()
{
	check(IsInGameThread());
	
	if (!bInitialized)
	{
		return;
	}
	
	FScopeLock _(&MasterALCCriticalSection);

	FCoreUObjectDelegates::GarbageCollectComplete.RemoveAll(this);

	if (MasterALC)
	{
		MasterALC->Unload();
	}

	for (const auto& Pair : SlimALCMap)
	{
		Pair.Value->Unload();
	}

	bInitialized = false;
}

void ZSharp::FZGenericCLR::CollectGarbage(int32 generation, bool bAggressive, bool bBlocking, bool bCompacting)
{
	ZCLR_Interop::GCollectGarbage(generation, bAggressive, bBlocking, bCompacting);
}

ZSharp::IZMasterAssemblyLoadContext* ZSharp::FZGenericCLR::CreateMasterALC()
{
	check(IsInGameThread());

	FScopeLock _(&MasterALCCriticalSection);
	
	if (MasterALC)
	{
		UE_LOG(LogZSharpCLR, Fatal, TEXT("Master ALC already exists!"));
	}

	FZGCHandle handle = ZCLR_Interop::GCreateMasterALC();
	if (!IsValid(handle))
	{
		return nullptr;
	}
	
	MasterALC = MakeUnique<FZMasterAssemblyLoadContext>(handle, [this]{ HandleMasterALCUnloaded(); });

	OnMasterALCLoaded.Broadcast(MasterALC.Get());

	return MasterALC.Get();
}

ZSharp::IZMasterAssemblyLoadContext* ZSharp::FZGenericCLR::GetMasterALC()
{
	check(IsInGameThread());
	
	FScopeLock _(&MasterALCCriticalSection);
	
	return MasterALC.Get();
}

void ZSharp::FZGenericCLR::GetMasterALC_AnyThread(TFunctionRef<void(IZMasterAssemblyLoadContext*)> action)
{
	FScopeLock _(&MasterALCCriticalSection);

	if (MasterALC)
	{
		action(MasterALC.Get());
	}
}

ZSharp::IZSlimAssemblyLoadContext* ZSharp::FZGenericCLR::CreateSlimALC(const FString& name)
{
	FWriteScopeLock _(SlimALCMapLock);
	
	if (SlimALCMap.Contains(name))
	{
		UE_LOG(LogZSharpCLR, Fatal, TEXT("Slim ALC [%s] already exists!"), *name);
	}

	FZGCHandle handle = ZCLR_Interop::GCreateSlimALC(*name);
	if (!IsValid(handle))
	{
		return nullptr;
	}

	IZSlimAssemblyLoadContext* alc = new FZSlimAssemblyLoadContext { handle, [this, name]{ HandleSlimALCUnloaded(name); }, name };
	SlimALCMap.Emplace(name, alc);
	
	return alc;
}

ZSharp::IZSlimAssemblyLoadContext* ZSharp::FZGenericCLR::GetSlimALC(const FString& name)
{
	FReadScopeLock _(SlimALCMapLock);
	
	const TUniquePtr<IZSlimAssemblyLoadContext>* pAlc = SlimALCMap.Find(name);
	return pAlc ? pAlc->Get() : nullptr;
}

void ZSharp::FZGenericCLR::HandleMasterALCUnloaded()
{
	OnMasterALCUnloaded.Broadcast();
	
	MasterALC = nullptr;
}

void ZSharp::FZGenericCLR::HandleSlimALCUnloaded(const FString& name)
{
	FWriteScopeLock _(SlimALCMapLock);
	
	SlimALCMap.Remove(name);
}

FDelegateHandle ZSharp::FZGenericCLR::RegisterMasterALCLoaded(FZOnMasterALCLoaded::FDelegate delegate, bool bNotifyIfLoaded)
{
	check(IsInGameThread());

	FScopeLock _(&MasterALCCriticalSection);
	
	FDelegateHandle handle = OnMasterALCLoaded.Add(delegate);
	if (bNotifyIfLoaded && MasterALC)
	{
		delegate.ExecuteIfBound(MasterALC.Get());
	}

	return handle;
}

void ZSharp::FZGenericCLR::UnregisterMasterALCLoaded(FDelegateHandle delegate)
{
	check(IsInGameThread());
	
	OnMasterALCLoaded.Remove(delegate);
}

void ZSharp::FZGenericCLR::UnregisterMasterALCLoaded(const void* userObject)
{
	check(IsInGameThread());
	
	OnMasterALCLoaded.RemoveAll(userObject);
}

FDelegateHandle ZSharp::FZGenericCLR::RegisterMasterALCUnloaded(FZOnMasterALCUnloaded::FDelegate delegate)
{
	check(IsInGameThread());
	
	return OnMasterALCUnloaded.Add(delegate);
}

void ZSharp::FZGenericCLR::UnregisterMasterALCUnloaded(FDelegateHandle delegate)
{
	check(IsInGameThread());
	
	OnMasterALCUnloaded.Remove(delegate);
}

void ZSharp::FZGenericCLR::UnregisterMasterALCUnloaded(const void* userObject)
{
	check(IsInGameThread());
	
	OnMasterALCUnloaded.RemoveAll(userObject);
}

void ZSharp::FZGenericCLR::HandleGarbageCollectComplete()
{
	CollectGarbage();
}


