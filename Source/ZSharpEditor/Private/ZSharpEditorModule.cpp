// Copyright Zero Games. All Rights Reserved.

#include "ZSharpEditorModule.h"

#include "IZExportedTypeRegistry.h"
#include "ZSharpRuntimeSettings.h"
#include "ALC/ZCommonDllMainArgs.h"
#include "CLR/IZSharpClr.h"
#include "Dynamic/ZDynamicExportedEnum.h"
#include "Interfaces/IPluginManager.h"
#include "Static/ZStaticExportedEnum.h"

ZSHARP_DECLARE_EXPORTED_ENUM(EForceInit, EForceInit, Core)

ZSHARP_BEGIN_EXPORT_ENUM(EForceInit)
	ZSHARP_EXPORT_ENUM_VALUE(ForceInit)
	ZSHARP_EXPORT_ENUM_VALUE(ForceInitToZero)
ZSHARP_END_EXPORT_ENUM(EForceInit)

namespace ZSharp::FZSharpEditorModule_Private
{
	static FAutoConsoleCommand GCmdZSharpGen(
		TEXT("zsharp.Gen"),
		TEXT("Generate zsharp solution and project files."),
		FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& args)
		{
			const FString pluginDir = IPluginManager::Get().GetModuleOwnerPlugin(UE_MODULE_NAME)->GetBaseDir();
			
			FString dllPath = FPaths::Combine(FPaths::ProjectDir(), "Binaries/Managed/ZeroGames.ZSharp.Build.dll");
			if (!FPaths::FileExists(dllPath))
			{
				dllPath = FPaths::Combine(pluginDir, "Content/ZeroGames.ZSharp.Build.dll");
				check(FPaths::FileExists(dllPath));
			}

			FString projectDirArg = FString::Printf(TEXT("projectdir=%s"), *FPaths::ConvertRelativePathToFull(FPaths::ProjectDir()));
			FString projectManagedSourceDir = FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::ProjectDir(), "Source/Managed"));
			FString pluginManagedSourceDir = FPaths::ConvertRelativePathToFull(FPaths::Combine(pluginDir, "Source/Managed"));
			FString sourceArg = FString::Printf(TEXT("source=%s"), *FString::Printf(TEXT("%s;%s"), *projectManagedSourceDir, *pluginManagedSourceDir));
			const TCHAR* argv[] =
			{
				TEXT("target=solution"),
				*projectDirArg,
				*sourceArg,
			};
			FZCommonDllMainArgs commonArgs { UE_ARRAY_COUNT(argv), argv };
			IZSharpClr::Get().Run(dllPath, &commonArgs);
		}),
		ECVF_Default);

	static FAutoConsoleCommand GCmdZSharpGlue(
		TEXT("zsharp.Glue"),
		TEXT("Generate zsharp glue code."),
		FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& args)
		{
			for (TObjectIterator<UEnum> It; It; ++It)
			{
				UEnum* enm = *It;
				if (!enm->IsNative())
				{
					continue;
				}

				FString module;
				bool bSuc = enm->GetPackage()->GetName().Split("/", nullptr, &module, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
				if (!bSuc || !GetDefault<UZSharpRuntimeSettings>()->IsModuleMapped(module))
				{
					continue;
				}

				const auto* exportedEnum = new FZDynamicExportedEnum { enm };
				if (!exportedEnum->IsRegistered())
				{
					delete exportedEnum;
				}
			}

			IZExportedTypeRegistry::Get().ForeachExportedEnum([](IZExportedEnum& enm)
			{
				UE_LOG(LogTemp, Error, TEXT("Exported Enum Name: [%s] Module: [%s] SlotType: [%s] UnderlyingType: [%s]"),
					*enm.GetName(),
					*enm.GetModule(),
					*ToString(enm.GetSlotType()),
					*enm.GetUnderlyingType());
			});
		}),
		ECVF_Default);
}

class FZSharpEditorModule : public IZSharpEditorModule
{
	// Begin IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// End IModuleInterface
};

IMPLEMENT_MODULE(FZSharpEditorModule, ZSharpEditor)

void FZSharpEditorModule::StartupModule()
{
}

void FZSharpEditorModule::ShutdownModule()
{
}
