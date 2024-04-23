// Copyright Zero Games. All Rights Reserved.

#include "ZSharpCLRModule.h"

#include "CLR/ZSharpCLR.h"
#include "CLR/IZSharpCLR.h"
#include "Interop/IZMasterAssemblyLoadContext.h"
#include "Interop/ZCommonDllMainArgs.h"

namespace ZSharp::ZSharpCLRModule_Private
{
	static FAutoConsoleCommand GCmdDotnetRun(
	TEXT("dotnet.Run"),
	TEXT("Load assembly of the specified path to an isolated slim assembly load context and invoke __DllEntry.DllMain(). Unloads the context when return from managed code."),
	FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& Args)
	{
		if (Args.IsEmpty())
		{
			return;
		}

		const FString& path = Args[0];
		TArray<const TCHAR*> argv;
		for (int32 i = 1; i < Args.Num(); ++i)
		{
			argv.Emplace(*Args[i]);
		}
		
		FZCommonDllMainArgs args { argv.Num(), argv.GetData() };
		IZSharpCLR::Get().Run(path, &args);
	}),
	ECVF_Default);

	static FAutoConsoleCommand GCmdDotnetRunAsync(
		TEXT("dotnet.RunAsync"),
		TEXT("Load assembly of the specified path to an isolated slim assembly load context and invoke __DllEntry.DllMain(). Requires user code to unload the context manually. Often used when need to await something."),
		FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& Args)
		{
			if (Args.IsEmpty())
			{
				return;
			}

			const FString& path = Args[0];
			TArray<const TCHAR*> argv;
			for (int32 i = 1; i < Args.Num(); ++i)
			{
				argv.Emplace(*Args[i]);
			}
		
			FZCommonDllMainArgs args { argv.Num(), argv.GetData() };
			IZSharpCLR::Get().RunAsync(path, &args);
		}),
		ECVF_Default);
}

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


