// Copyright Zero Games. All Rights Reserved.


#include "CLR/ZGenericCLR.h"

#include "hostfxr.h"
#include "coreclr_delegates.h"
#include "ZSharpCLRLogChannels.h"
#include "Interfaces/IPluginManager.h"
#include "ALC/IZSlimAssemblyLoadContext.h"
#include "Interop/UnrealEngine_Interop.h"
#include "Interop/ZAssembly_Interop.h"
#include "ZCall/ZCallBuffer.h"
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
	static void LoadCoreAssembly(const FString& pluginBinariesDir, load_assembly_bytes_fn loadAssembly, get_function_pointer_fn getFunctionPointer)
	{
		constexpr struct
		{
			struct
			{
				void (*UnrealEngine_Log)(uint8, const TCHAR*) = FZUnrealEngine_Interop::Log;
				uint8 (*UnrealEngine_IsInGameThread)() = FZUnrealEngine_Interop::IsInGameThread;

				FString&(*InteropString_Alloc)(const TCHAR*) = FZInteropString_Interop::Alloc;
				void (*InteropString_Free)(FString&) = FZInteropString_Interop::Free;
				const TCHAR*(*InteropString_GetData)(FString&) = FZInteropString_Interop::GetData;
				void (*InteropString_SetData)(FString&, const TCHAR*) = FZInteropString_Interop::SetData;

				int32 (*MasterAssemblyLoadContext_ZCallByHandle)(FZCallHandle, FZCallBuffer*) = FZMasterAssemblyLoadContext_Interop::ZCallByHandle;
				int32 (*MasterAssemblyLoadContext_ZCallByName)(const TCHAR*, FZCallBuffer*, FZCallHandle*) = FZMasterAssemblyLoadContext_Interop::ZCallByName;
				void (*MasterAssemblyLoadContext_ZCallByHandle_AnyThread)(FZCallHandle, FZCallBuffer*, int32) = FZMasterAssemblyLoadContext_Interop::ZCallByHandle_AnyThread;
				FZCallHandle (*MasterAssemblyLoadContext_GetZCallHandle)(const TCHAR*) = FZMasterAssemblyLoadContext_Interop::GetZCallHandle;
			} UnmanagedFunctions;
		} input;

		struct
		{
			struct
			{
				void (*CLR_CollectGarbage)(int32, uint8, uint8, uint8);
				FZGCHandle (*CLR_CreateMasterALC)();
				FZGCHandle (*CLR_CreateSlimALC)(const TCHAR*);

				int32 (*GCHandle_Free)(FZGCHandle);

				int32 (*MasterAssemblyLoadContext_Unload)();
				FZGCHandle (*MasterAssemblyLoadContext_LoadAssembly)(const uint8*, int32, void*);
				int32 (*MasterAssemblyLoadContext_ReleaseConjugate)(FZConjugateHandle);

				int32 (*SlimAssemblyLoadContext_Unload)(FZGCHandle);
				int32 (*SlimAssemblyLoadContext_LoadAssembly)(FZGCHandle, const uint8*, int32, void*);
				int32 (*SlimAssemblyLoadContext_CallMethod)(FZGCHandle, const TCHAR*, const TCHAR*, const TCHAR*, void*);

				int32 (*Assembly_GetName)(FZGCHandle, FString&);
				FZGCHandle (*Assembly_GetType)(FZGCHandle, const TCHAR*);

				int32 (*Type_GetName)(FZGCHandle, FString&);
				FZConjugateHandle (*Type_BuildConjugate)(FZGCHandle, void*);
				FZGCHandle (*Type_GetMethodInfo)(FZGCHandle, const TCHAR*);
				FZGCHandle (*Type_GetPropertyInfo)(FZGCHandle, const TCHAR*);

				int32 (*MethodInfo_GetName)(FZGCHandle, FString&);
				int32 (*MethodInfo_GetNumSlots)(FZGCHandle);
				int32 (*MethodInfo_Invoke)(FZGCHandle, FZCallBuffer*);
			} ManagedFunctions;
		} output{};

		int32(*dllMain)(const decltype(input)&, const decltype(output)&) = nullptr;

		const FString assemblyName = ZSHARP_CORE_ASSEMBLY_NAME;
		const FString assemblyPath = FPaths::Combine(pluginBinariesDir, "Managed", assemblyName + ".dll");
		const FString entryTypeName = FString::Printf(TEXT("%s.DllEntry, %s"), *assemblyName, *assemblyName);
		const FString entryMethodName = TEXT("DllMain");

		TArray<uint8> content;
		FFileHelper::LoadFileToArray(content, *assemblyPath, FILEREAD_Silent);
		loadAssembly(content.GetData(), content.Num(), nullptr, 0, nullptr, nullptr);
		getFunctionPointer(*entryTypeName, *entryMethodName, UNMANAGEDCALLERSONLY_METHOD, nullptr, nullptr, reinterpret_cast<void**>(&dllMain));

		check(dllMain);
		dllMain(input, output);

		ZCLR_Interop::GCollectGarbage = output.ManagedFunctions.CLR_CollectGarbage;
		ZCLR_Interop::GCreateMasterALC = output.ManagedFunctions.CLR_CreateMasterALC;
		ZCLR_Interop::GCreateSlimALC = output.ManagedFunctions.CLR_CreateSlimALC;

		FZGCHandle_Interop::GFree = output.ManagedFunctions.GCHandle_Free;

		FZMasterAssemblyLoadContext_Interop::GUnload = output.ManagedFunctions.MasterAssemblyLoadContext_Unload;
		FZMasterAssemblyLoadContext_Interop::GLoadAssembly = output.ManagedFunctions.
		                                                            MasterAssemblyLoadContext_LoadAssembly;
		FZMasterAssemblyLoadContext_Interop::GReleaseConjugate = output.ManagedFunctions.
		                                                                MasterAssemblyLoadContext_ReleaseConjugate;

		FZSlimAssemblyLoadContext_Interop::GUnload = output.ManagedFunctions.SlimAssemblyLoadContext_Unload;
		FZSlimAssemblyLoadContext_Interop::GLoadAssembly = output.ManagedFunctions.SlimAssemblyLoadContext_LoadAssembly;
		FZSlimAssemblyLoadContext_Interop::GCallMethod = output.ManagedFunctions.SlimAssemblyLoadContext_CallMethod;

		FZAssembly_Interop::GGetName = output.ManagedFunctions.Assembly_GetName;
		FZAssembly_Interop::GGetType = output.ManagedFunctions.Assembly_GetType;

		FZType_Interop::GGetName = output.ManagedFunctions.Type_GetName;
		FZType_Interop::GBuildConjugate = output.ManagedFunctions.Type_BuildConjugate;
		FZType_Interop::GGetMethodInfo = output.ManagedFunctions.Type_GetMethodInfo;
		FZType_Interop::GGetPropertyInfo = output.ManagedFunctions.Type_GetPropertyInfo;

		FZMethodInfo_Interop::GGetName = output.ManagedFunctions.MethodInfo_GetName;
		FZMethodInfo_Interop::GGetNumSlots = output.ManagedFunctions.MethodInfo_GetNumSlots;
		FZMethodInfo_Interop::GInvoke = output.ManagedFunctions.MethodInfo_Invoke;
	}

	static void LoadEngineCoreAssembly(const FString& pluginBinariesDir, load_assembly_bytes_fn loadAssembly, get_function_pointer_fn getFunctionPointer)
	{
		const TCHAR* typeName = TEXT("ZeroGames.ZSharp.UnrealEngine.Core.UnrealEngine_Interop");

		// This exists because UE_ARRAY_COUNT() doesn't work with static initialized member array.
		struct FZArgv
		{
			const TCHAR* TypeName;
			const TCHAR* FieldName;
			void* Function;
		} argv[] =
		{
			{ typeName, TEXT("SLog"), FZUnrealEngine_Interop::Log },
			{ typeName, TEXT("SIsInGameThread"), FZUnrealEngine_Interop::IsInGameThread },
		};
		
		struct
        {
        	int32 Argc = UE_ARRAY_COUNT(argv);
        	FZArgv* Argv = argv;
        } args{};

		int32(*dllMain)(const decltype(args)&) = nullptr;

		const FString assemblyName = ZSHARP_ENGINE_CORE_ASSEMBLY_NAME;
		const FString assemblyPath = FPaths::Combine(pluginBinariesDir, "Managed", assemblyName + ".dll");
		const FString entryTypeName = FString::Printf(TEXT("%s.DllEntry, %s"), *assemblyName, *assemblyName);
		const FString entryMethodName = TEXT("DllMain");

		TArray<uint8> content;
		FFileHelper::LoadFileToArray(content, *assemblyPath, FILEREAD_Silent);
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
	getRuntimeDelegate(handle, hdt_load_assembly_bytes, (void**)&loadAssembly);
	check(loadAssembly);

	get_function_pointer_fn getFunctionPointer;
	getRuntimeDelegate(handle, hdt_get_function_pointer, (void**)&getFunctionPointer);

	closeHostFXR(handle);

	FZGenericCLR_Private::LoadCoreAssembly(pluginBinariesDir, loadAssembly, getFunctionPointer);
	FZGenericCLR_Private::LoadEngineCoreAssembly(pluginBinariesDir, loadAssembly, getFunctionPointer);
	
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


