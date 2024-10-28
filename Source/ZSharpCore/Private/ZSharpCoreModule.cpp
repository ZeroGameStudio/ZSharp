// Copyright Zero Games. All Rights Reserved.

#include "ZSharpCoreModule.h"

#include "CLR/ZSharpClr.h"
#include "CLR/IZSharpClr.h"
#include "ALC/ZCommonMethodArgs.h"

namespace ZSharp::ZSharpCoreModule_Private
{
	static FAutoConsoleCommand GCmdZSharpRun(
	TEXT("z#.run"),
	TEXT("Load assembly of the specified path to an isolated slim assembly load context and invoke __DllEntry.DllMain(). Unloads the context when return from managed code."),
	FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& args)
	{
		if (args.IsEmpty())
		{
			return;
		}

		const FString& name = args[0];
		TArray<const TCHAR*> argv;
		for (int32 i = 1; i < args.Num(); ++i)
		{
			argv.Emplace(*args[i]);
		}
		
		FZCommonMethodArgs commonArgs { argv.Num(), argv.GetData() };
		IZSharpClr::Get().Run(name, &commonArgs);
	}),
	ECVF_Default);

	static FAutoConsoleCommand GCmdZSharpRunAsync(
		TEXT("z#.runa"),
		TEXT("Load assembly of the specified path to an isolated slim assembly load context and invoke __DllEntry.DllMain(). Requires user code to unload the context manually. Often used when need to await something."),
		FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& args)
		{
			if (args.IsEmpty())
			{
				return;
			}

			const FString& name = args[0];
			TArray<const TCHAR*> argv;
			for (int32 i = 1; i < args.Num(); ++i)
			{
				argv.Emplace(*args[i]);
			}
		
			FZCommonMethodArgs commonArgs { argv.Num(), argv.GetData() };
			IZSharpClr::Get().RunAsync(name, &commonArgs);
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
	ZSharp::FZSharpClr::Get().Startup();
}

void FZSharpCoreModule::ShutdownModule()
{
	ZSharp::FZSharpClr::Get().Shutdown();
}


