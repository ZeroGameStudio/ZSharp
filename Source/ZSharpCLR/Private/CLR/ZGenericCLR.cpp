// Copyright Zero Games. All Rights Reserved.


#include "CLR/ZGenericCLR.h"

#include "hostfxr.h"
#include "coreclr_delegates.h"
#include "Interop/IZSlimAssemblyLoadContext.h"
#include "Interop/ZAssembly_Interop.h"
#include "Interop/ZCallBuffer.h"
#include "Interop/ZCLR_Interop.h"
#include "Interop/ZGCHandle_Interop.h"
#include "Interop/ZInteropString_Interop.h"
#include "Interop/ZMasterAssemblyLoadContext.h"
#include "Interop/ZMasterAssemblyLoadContext_Interop.h"
#include "Interop/ZGCHandle.h"

namespace ZSharp::FZGenericCLR_Private
{
	static void UE_Log(uint8 level, const TCHAR* message)
	{
		switch (level)
		{
		case 1:
			{
				UE_LOG(LogTemp, Log, TEXT("%s"), message);
				break;
			}
		case 2:
			{
				UE_LOG(LogTemp, Warning, TEXT("%s"), message);
				break;
			}
		case 3:
			{
				UE_LOG(LogTemp, Error, TEXT("%s"), message);
				break;
			}
		default:
			{
				break;
			}
		}
	}
}

FDelegateHandle ZSharp::FZGenericCLR::RegisterMasterALCLoaded(FZOnMasterALCLoaded::FDelegate delegate, bool bNotifyIfLoaded)
{
	FDelegateHandle handle = OnMasterALCLoaded.Add(delegate);
	if (bNotifyIfLoaded && MasterALC.Get())
	{
		delegate.ExecuteIfBound(MasterALC.Get());
	}

	return handle;
}

void ZSharp::FZGenericCLR::UnregisterMasterALCLoaded(FDelegateHandle delegate)
{
	OnMasterALCLoaded.Remove(delegate);
}

void ZSharp::FZGenericCLR::UnregisterMasterALCLoaded(const void* userObject)
{
	OnMasterALCLoaded.RemoveAll(userObject);
}

FDelegateHandle ZSharp::FZGenericCLR::RegisterMasterALCUnloaded(FZOnMasterALCUnloaded::FDelegate delegate)
{
	return OnMasterALCUnloaded.Add(delegate);
}

void ZSharp::FZGenericCLR::UnregisterMasterALCUnloaded(FDelegateHandle delegate)
{
	OnMasterALCUnloaded.Remove(delegate);
}

void ZSharp::FZGenericCLR::UnregisterMasterALCUnloaded(const void* userObject)
{
	OnMasterALCUnloaded.RemoveAll(userObject);
}

void ZSharp::FZGenericCLR::Startup()
{
	if (bInitialized)
	{
		return;
	}

	bInitialized = true;
	
	const FString pluginDir = FPaths::Combine(FPaths::ProjectPluginsDir(), "ZeroGames", "ZSharp");
	const FString pluginBinariesDir = FPaths::Combine(pluginDir, "Binaries");
	const FString pluginContentDir = FPaths::Combine(pluginDir, "Content");
	const FString platformName = "Win64";
	const FString dotnetRoot = FPaths::Combine(pluginBinariesDir, platformName, "dotnet");
	
	const FString hostFXRPath = FPaths::Combine(dotnetRoot, "host", "fxr", "8.0.3", "hostfxr.dll");
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
	const FString runtimeConfigPath = FPaths::Combine(pluginContentDir, TEXT("ZSharp.runtimeconfig.json"));
	initializeHostFxr(*runtimeConfigPath, nullptr, &handle);
	check(handle);

	load_assembly_bytes_fn loadAssembly;
	getRuntimeDelegate(handle, hdt_load_assembly_bytes, (void**)&loadAssembly);
	check(loadAssembly);

	get_function_pointer_fn getFunctionPointer;
	getRuntimeDelegate(handle, hdt_get_function_pointer, (void**)&getFunctionPointer);

	closeHostFXR(handle);

	const FString assembly = FPaths::Combine(pluginBinariesDir, "Managed", "ZeroGames.ZSharp.Core.dll");
	const FString type = TEXT("ZeroGames.ZSharp.Core.Entry, ZeroGames.ZSharp.Core");
	const FString method = TEXT("Startup");

	constexpr struct FStartupInput
	{
		struct
		{
			void(*UE_Log)(uint8, const TCHAR*) = FZGenericCLR_Private::UE_Log;
			
			FString&(*InteropString_Alloc)(const TCHAR*) = FZInteropString_Interop::Alloc;
			void(*InteropString_Free)(FString&) = FZInteropString_Interop::Free;
			const TCHAR*(*InteropString_GetData)(FString&) = FZInteropString_Interop::GetData;
			void(*InteropString_SetData)(FString&, const TCHAR*) = FZInteropString_Interop::SetData;

			int32(*MasterAssemblyLoadContext_ZCallByHandle)(FZCallHandle, FZCallBuffer*) = FZMasterAssemblyLoadContext_Interop::ZCallByHandle;
			int32(*MasterAssemblyLoadContext_ZCallByName)(const TCHAR*, FZCallBuffer*, FZCallHandle*, uint8) = FZMasterAssemblyLoadContext_Interop::ZCallByName;
			FZCallHandle(*MasterAssemblyLoadContext_GetZCallHandle)(const TCHAR*) = FZMasterAssemblyLoadContext_Interop::GetZCallHandle;
		} UnmanagedFunctions;
	} input;
	
	struct FStartupOutput
	{
		struct
		{
			FZGCHandle(*CLR_CreateMasterALC)();
			
			void(*GCHandle_Free)(FZGCHandle);

			void(*MasterAssemblyLoadContext_Unload)();
			FZGCHandle(*MasterAssemblyLoadContext_LoadAssembly)(const uint8*, int32, void*);

			uint8(*Assembly_GetName)(FZGCHandle, FString&);
		} ManagedFunctions;
	} output{};
	
	int(*startUp)(const FStartupInput&, FStartupOutput&) = nullptr;

	TArray<uint8> content;
	FFileHelper::LoadFileToArray(content, *assembly, FILEREAD_Silent);
	loadAssembly(content.GetData(), content.Num(), nullptr, 0, nullptr, nullptr);
	getFunctionPointer(*type, *method, UNMANAGEDCALLERSONLY_METHOD, nullptr, nullptr, (void**)&startUp);

	check(startUp);
	startUp(input, output);

	ZCLR_Interop::GCreateMasterALC = output.ManagedFunctions.CLR_CreateMasterALC;

	FZGCHandle_Interop::GFree = output.ManagedFunctions.GCHandle_Free;

	FZMasterAssemblyLoadContext_Interop::GUnload = output.ManagedFunctions.MasterAssemblyLoadContext_Unload;
	FZMasterAssemblyLoadContext_Interop::GLoadAssembly = output.ManagedFunctions.MasterAssemblyLoadContext_LoadAssembly;

	FZAssembly_Interop::GGetName = output.ManagedFunctions.Assembly_GetName;
}

ZSharp::IZMasterAssemblyLoadContext* ZSharp::FZGenericCLR::LoadMasterALC()
{
	if (MasterALC)
	{
		UE_LOG(LogTemp, Fatal, TEXT("Master ALC already exists!"));
	}

	FZGCHandle handle = ZCLR_Interop::GCreateMasterALC();
	MasterALC = MakeUnique<FZMasterAssemblyLoadContext>(handle, [this]{ HandleMasterALCUnloaded(); });

	OnMasterALCLoaded.Broadcast(MasterALC.Get());

	return MasterALC.Get();
}

void ZSharp::FZGenericCLR::UnloadMasterALC()
{
	if (MasterALC.Get())
	{
		MasterALC->Unload();
	}
}

ZSharp::IZMasterAssemblyLoadContext* ZSharp::FZGenericCLR::GetMasterALC()
{
	return MasterALC.Get();
}

ZSharp::IZSlimAssemblyLoadContext* ZSharp::FZGenericCLR::LoadSlimALC(const FString& name)
{
	checkNoEntry();
	return nullptr;
}

void ZSharp::FZGenericCLR::UnloadSlimALC(const FString& name)
{
	if (IZSlimAssemblyLoadContext* alc = GetSlimALC(name))
	{
		alc->Unload();
	}
}

void ZSharp::FZGenericCLR::UnloadSlimALC(IZSlimAssemblyLoadContext* alc)
{
	alc->Unload();
}

ZSharp::IZSlimAssemblyLoadContext* ZSharp::FZGenericCLR::GetSlimALC(const FString& name)
{
	checkNoEntry();
	return nullptr;
}

void ZSharp::FZGenericCLR::HandleMasterALCUnloaded()
{
	MasterALC = nullptr;

	OnMasterALCUnloaded.Broadcast();
}


