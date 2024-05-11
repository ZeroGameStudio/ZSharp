// Copyright Zero Games. All Rights Reserved.

#include "ZSharpCoreModule.h"

#include "CLR/ZSharpClr.h"
#include "CLR/IZSharpClr.h"
#include "ALC/ZCommonDllMainArgs.h"

namespace ZSharp::ZSharpCoreModule_Private
{
	static FAutoConsoleCommand GCmdZSharpRun(
	TEXT("zs.run"),
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
		TEXT("zs.runa"),
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

class FZSharpCoreModule : public IZSharpCoreModule
{
	// Begin IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// End IModuleInterface
};

IMPLEMENT_MODULE(FZSharpCoreModule, ZSharpCore)

void FZSharpCoreModule::StartupModule()
{
	FZSharpClr::Get().Startup();
}

void FZSharpCoreModule::ShutdownModule()
{
	FZSharpClr::Get().Shutdown();
}


