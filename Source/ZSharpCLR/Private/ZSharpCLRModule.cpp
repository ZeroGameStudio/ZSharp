// Copyright Zero Games. All Rights Reserved.

#include "ZSharpCLRModule.h"

#include "CLR\ZSharpCLR.h"
#include "CLR/IZSharpCLR.h"
#include "Interop/IZMasterAssemblyLoadContext.h"

static uint64 GAnonymousSlimALCNameSerial = 0;

static FAutoConsoleCommand CDotnetRun(
	TEXT("dotnet.Run"),
	TEXT("Load assembly of the specified path to an isolated slim assembly load context and invoke __DllEntry.DllMain(). Unloads the context when return from managed code."),
	FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& Args)
	{
		if (Args.IsEmpty())
		{
			return;
		}

		ZSharp::IZSlimAssemblyLoadContext* alc = ZSharp::IZSharpCLR::Get().CreateSlimALC(FString::Printf(TEXT("__Anonymous%llu"), GAnonymousSlimALCNameSerial++));
		if (!alc)
		{
			return;
		}
		
		TArray<uint8> content;
		if (!FFileHelper::LoadFileToArray(content, *Args[0], FILEREAD_Silent))
		{
			return;
		}

		TArray<const TCHAR*> argv;
		for (int32 i = 1; i < Args.Num(); ++i)
		{
			argv.Emplace(*Args[i]);
		}
		
		struct FZArgs
		{
			int32 Argc = argv.Num();
			const TCHAR** Argv = argv.GetData();
		} args{};

		alc->LoadAssembly(content, &args);
		ZSharp::IZSharpCLR::Get().UnloadSlimALC(alc);
	}),
	ECVF_Default);

static FAutoConsoleCommand CDotnetRunAsync(
	TEXT("dotnet.RunAsync"),
	TEXT("Load assembly of the specified path to an isolated slim assembly load context and invoke __DllEntry.DllMain(). Requires user code to unload the context manually. Often used when need to await something."),
	FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& Args)
	{

	}),
	ECVF_Default);

class FZSharpCLRModule : public IZSharpCLRModule
{
	// Begin IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// End IModuleInterface
};

IMPLEMENT_MODULE(FZSharpCLRModule, ZSharpCLR)

void FZSharpCLRModule::StartupModule()
{
	FZSharpCLR::Get().Startup();
}

void FZSharpCLRModule::ShutdownModule()
{
	if (ZSharp::IZMasterAssemblyLoadContext* alc = ZSharp::IZSharpCLR::Get().GetMasterALC())
	{
		alc->Unload();
	}
}


