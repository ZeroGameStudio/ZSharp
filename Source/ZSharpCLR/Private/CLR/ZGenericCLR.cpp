// Copyright Zero Games. All Rights Reserved.


#include "CLR/ZGenericCLR.h"

#include "hostfxr.h"
#include "coreclr_delegates.h"
#include "Interop/ZCLR_Interop.h"
#include "Interop/ZGCHandle_Interop.h"

namespace ZSharp::FZGenericCLR_Private
{
	static void UE_Log(uint8 level, const TCHAR* message)
	{
		switch (level)
		{
		case 1:
			{
				UE_LOG(LogTemp, Log, TEXT("%s"), message);
			}
		case 2:
			{
				UE_LOG(LogTemp, Warning, TEXT("%s"), message);
			}
		case 3:
			{
				UE_LOG(LogTemp, Error, TEXT("%s"), message);
			}
		default:
			{
				break;
			}
		}
	}
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

	const FString assembly = FPaths::Combine(pluginBinariesDir, "Managed", "ZeroGames.ZSharp.dll");
	const FString type = TEXT("ZeroGames.ZSharp.Entry, ZeroGames.ZSharp");
	const FString method = TEXT("Startup");

	struct FStartupInput
	{
		struct
		{
			void(*UE_Log)(uint8, const TCHAR*) = FZGenericCLR_Private::UE_Log;
		} UnmanagedFunctions;
	} input;

	struct FStartupOutput
	{
		struct
		{
			FZGCHandle(*CLR_CreateMasterALC)();
			
			void(*GCHandle_Free)(FZGCHandle);
		} ManagedFunctions;
		struct
		{
			FZGCHandle TypeofAssemblyProxy;
			FZGCHandle TypeofTypeProxy;
			FZGCHandle TypeofMethodInfoProxy;
			FZGCHandle TypeofPropertyInfoProxy;
			FZGCHandle TypeofAttributeProxy;
		} ManagedObjects;
	} output;
	
	int(*startUp)(const FStartupInput&, FStartupOutput&) = nullptr;

	TArray<uint8> content;
	FFileHelper::LoadFileToArray(content, *assembly, FILEREAD_Silent);
	loadAssembly(content.GetData(), content.Num(), nullptr, 0, nullptr, nullptr);
	getFunctionPointer(*type, *method, UNMANAGEDCALLERSONLY_METHOD, nullptr, nullptr, (void**)&startUp);

	check(startUp);
	startUp(input, output);

	ZCLR_Interop::GCreateMasterALC = output.ManagedFunctions.CLR_CreateMasterALC;

	FZGCHandle_Interop::GFree = output.ManagedFunctions.GCHandle_Free;
}

ZSharp::IZMasterAssemblyLoadContext* ZSharp::FZGenericCLR::CreateMasterALC()
{
	if (MasterALC)
	{
		UE_LOG(LogTemp, Fatal, TEXT("Master ALC already exists!"));
	}

	FZGCHandle Handle = ZCLR_Interop::GCreateMasterALC();

	return MasterALC.Get();
}

ZSharp::IZMasterAssemblyLoadContext* ZSharp::FZGenericCLR::GetMasterALC()
{
	return MasterALC.Get();
}

ZSharp::IZSlimAssemblyLoadContext* ZSharp::FZGenericCLR::CreateSlimALC(const FString& name)
{
	checkNoEntry();
	return nullptr;
}

ZSharp::IZSlimAssemblyLoadContext* ZSharp::FZGenericCLR::GetSlimALC(const FString& name)
{
	checkNoEntry();
	return nullptr;
}


