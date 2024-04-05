// Copyright Zero Games. All Rights Reserved.


#include "CLR/GenericCLR.h"

#include "hostfxr.h"
#include "coreclr_delegates.h"

static void UELog(TCHAR* Message)
{
	UE_LOG(LogTemp, Error, TEXT("%s"), Message);
}

static const TCHAR* GetString()
{
	UE_LOG(LogTemp, Error, TEXT("asdf"));
	return GetData(FString::Printf(TEXT("500")));
}

void ZSharp::FGenericCLR::Startup()
{
	const FString PluginDir = FPaths::Combine(FPaths::ProjectPluginsDir(), "ZeroGames", "ZSharp");
	const FString PluginBinariesDir = FPaths::Combine(PluginDir, "Binaries");
	const FString PluginContentDir = FPaths::Combine(PluginDir, "Content");
	const FString PlatformName = "Win64";
	const FString DotnetRoot = FPaths::Combine(PluginBinariesDir, PlatformName, "dotnet");
	
	const FString HostFXRPath = FPaths::Combine(DotnetRoot, "host", "fxr", "8.0.3", "hostfxr.dll");
	void* HostFXR = FPlatformProcess::GetDllHandle(*HostFXRPath);
	check(HostFXR);

	hostfxr_initialize_for_runtime_config_fn InitializeHostFxr = (hostfxr_initialize_for_runtime_config_fn)
		FPlatformProcess::GetDllExport(HostFXR, TEXT("hostfxr_initialize_for_runtime_config"));
	check(InitializeHostFxr);
	hostfxr_get_runtime_delegate_fn GetRuntimeDelegate = (hostfxr_get_runtime_delegate_fn)
		FPlatformProcess::GetDllExport(HostFXR, TEXT("hostfxr_get_runtime_delegate"));
	check(GetRuntimeDelegate);
	hostfxr_close_fn CloseHostFXR = (hostfxr_close_fn)FPlatformProcess::GetDllExport(HostFXR, TEXT("hostfxr_close"));
	check(CloseHostFXR);

	hostfxr_handle Handle = nullptr;
	const FString RuntimeConfigPath = FPaths::Combine(PluginContentDir, TEXT("ZSharp.runtimeconfig.json"));
	InitializeHostFxr(*RuntimeConfigPath, nullptr, &Handle);
	check(Handle);

	load_assembly_bytes_fn LoadAssembly;
	GetRuntimeDelegate(Handle, hdt_load_assembly_bytes, (void**)&LoadAssembly);
	check(LoadAssembly);

	get_function_pointer_fn GetFunctionPointer;
	GetRuntimeDelegate(Handle, hdt_get_function_pointer, (void**)&GetFunctionPointer);

	CloseHostFXR(Handle);

	const FString Assembly = FPaths::Combine(PluginBinariesDir, "Managed", "ZeroGames.ZSharp.dll");
	const FString Type = TEXT("ZeroGames.ZSharp.Entry, ZeroGames.ZSharp");
	const FString Method = TEXT("Startup");
	int(*Startup)() = nullptr;

	TArray<uint8> Content;
	FFileHelper::LoadFileToArray(Content, *Assembly, FILEREAD_Silent);
	LoadAssembly(Content.GetData(), Content.Num(), nullptr, 0, nullptr, nullptr);
	GetFunctionPointer(*Type, *Method, UNMANAGEDCALLERSONLY_METHOD, nullptr, nullptr, (void**)&Startup);

	check(Startup);
	int32 x = Startup();
	UE_LOG(LogTemp, Error, TEXT("%d"), x);
}
