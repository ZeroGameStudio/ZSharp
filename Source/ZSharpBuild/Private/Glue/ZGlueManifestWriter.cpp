// Copyright Zero Games. All Rights Reserved.


#include "ZGlueManifestWriter.h"

#include "IZExportedEnum.h"
#include "IZExportedTypeRegistry.h"
#include "ZSharpRuntimeSettings.h"
#include "Policies/PrettyJsonPrintPolicy.h"

namespace ZSharp::FZGlueManifestWriter_Private
{
	// FJsonObjectConverter is so fucking buggy that we have to write manually.
	TSharedRef<FJsonObject> AssemblyToJson(const FZExportedAssemblyDto& dto)
	{
		TSharedRef<FJsonObject> jsonObj = MakeShared<FJsonObject>();

		TArray<TSharedPtr<FJsonValue>> enums;
		for (const auto& enm : dto.Enums)
		{
			TSharedRef<FJsonObject> enmJsonObj = MakeShared<FJsonObject>();
			enmJsonObj->SetStringField("Name", enm.Name);
			enmJsonObj->SetStringField("Module", enm.Module);
			enmJsonObj->SetStringField("UnderlyingType", enm.UnderlyingType);
			{
				TSharedRef<FJsonObject> valueMapJsonObj = MakeShared<FJsonObject>();
				for (const auto& pair : enm.ValueMap)
				{
					valueMapJsonObj->SetStringField(pair.Key, pair.Value);
				}
				enmJsonObj->SetObjectField("ValueMap", valueMapJsonObj);
			}

			TSharedRef<FJsonValueObject> enmJsonValue = MakeShared<FJsonValueObject>(enmJsonObj);
			enums.Emplace(enmJsonValue);
		}

		jsonObj->SetArrayField("Enums", MoveTemp(enums));

		return jsonObj;
	}

	bool JsonToString(TSharedRef<FJsonObject> json, FString& outString)
	{
		TSharedRef<TJsonWriter<>> jw = TJsonWriterFactory<>::Create(&outString);
		ON_SCOPE_EXIT
		{
			jw->Close();
		};

		return FJsonSerializer::Serialize(json, jw);
	}
}

void ZSharp::FZGlueManifestWriter::Write()
{
	const UZSharpRuntimeSettings* settings = GetDefault<UZSharpRuntimeSettings>();
	IZExportedTypeRegistry::Get().ForeachExportedEnum([this, settings](const IZExportedEnum& enm)
	{
		UE_LOG(LogTemp, Log, TEXT("Exported Enum Name: [%s] Module: [%s] SlotType: [%s] UnderlyingType: [%s]"),
			*enm.GetName(),
			*enm.GetModule(),
			*ToString(enm.GetSlotType()),
			*enm.GetUnderlyingType());

		const FString module = enm.GetModule();
		FString assembly;
		if (!settings->TryGetModuleAssembly(module, assembly))
		{
			return;
		}

		TUniquePtr<FZExportedAssemblyDto>& assemblyDto = AssemblyDtoMap.FindOrAdd(assembly);
		if (!assemblyDto)
		{
			assemblyDto = MakeUnique<FZExportedAssemblyDto>();
		}
		
		FZExportedEnumDto enumDto;
		enumDto.Name = enm.GetName();
		enumDto.Module = module;
		enumDto.UnderlyingType = enm.GetUnderlyingType();
		enm.ForeachEnumValue([&enumDto](const FString& name, const FString& value)
		{
			enumDto.ValueMap.Emplace(name, value);
		});
			
		assemblyDto->Enums.Emplace(MoveTemp(enumDto));
	});

	const FString glueDir = FPaths::Combine(FPaths::ProjectDir(), "Intermediate/ZSharp/Glue");
	for (const auto& pair : AssemblyDtoMap)
	{
		TSharedRef<FJsonObject> jsonObj = FZGlueManifestWriter_Private::AssemblyToJson(*pair.Value);
		FString jsonStr;
		if (!FZGlueManifestWriter_Private::JsonToString(jsonObj, jsonStr))
		{
			checkNoEntry();
		}

		FFileHelper::SaveStringToFile(jsonStr, *FPaths::Combine(glueDir, pair.Key, "Manifest.json"));
	}
}


