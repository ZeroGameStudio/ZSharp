// Copyright Zero Games. All Rights Reserved.

#include "ZSharpCLRModule.h"

#include "CLR/ZSharpCLR.h"
#include "CLR/IZSharpClr.h"
#include "ALC/ZCommonDllMainArgs.h"

namespace ZSharp::ZSharpCLRModule_Private
{
	static FAutoConsoleCommand GCmdZSharpRun(
	TEXT("zsharp.Run"),
	TEXT("Load assembly of the specified path to an isolated slim assembly load context and invoke __DllEntry.DllMain(). Unloads the context when return from managed code."),
	FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& args)
	{
		if (args.IsEmpty())
		{
			return;
		}

		const FString& path = args[0];
		TArray<const TCHAR*> argv;
		for (int32 i = 1; i < args.Num(); ++i)
		{
			argv.Emplace(*args[i]);
		}
		
		FZCommonDllMainArgs commonArgs { argv.Num(), argv.GetData() };
		IZSharpClr::Get().Run(path, &commonArgs);
	}),
	ECVF_Default);

	static FAutoConsoleCommand GCmdZSharpRunAsync(
		TEXT("zsharp.RunAsync"),
		TEXT("Load assembly of the specified path to an isolated slim assembly load context and invoke __DllEntry.DllMain(). Requires user code to unload the context manually. Often used when need to await something."),
		FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& args)
		{
			if (args.IsEmpty())
			{
				return;
			}

			const FString& path = args[0];
			TArray<const TCHAR*> argv;
			for (int32 i = 1; i < args.Num(); ++i)
			{
				argv.Emplace(*args[i]);
			}
		
			FZCommonDllMainArgs commonArgs { argv.Num(), argv.GetData() };
			IZSharpClr::Get().RunAsync(path, &commonArgs);
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
	FZSharpClr::Get().Startup();
}

void FZSharpCLRModule::ShutdownModule()
{
	FZSharpClr::Get().Shutdown();
}


