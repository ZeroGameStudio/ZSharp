// Copyright Zero Games. All Rights Reserved.


#include "ZGlueManifestWriter.h"

#include "IZExportedEnum.h"
#include "IZExportedTypeRegistry.h"
#include "ZSharpExportSettings.h"
#include "DTO/ZExportedClassDto.h"
#include "Policies/PrettyJsonPrintPolicy.h"

namespace ZSharp::ZGlueManifestWriter_Private
{
	// FJsonObjectConverter is so fucking buggy that we have to write manually.
	TSharedRef<FJsonObject> AssemblyToJson(const FZExportedAssemblyDto& dto)
	{
		auto jsonObj = MakeShared<FJsonObject>();

		{ // Enums
			TArray<TSharedPtr<FJsonValue>> enums;
			for (const auto& enm : dto.Enums)
			{
				auto enmJsonObj = MakeShared<FJsonObject>();
				enmJsonObj->SetStringField("Name", enm.Name);
				enmJsonObj->SetStringField("Module", enm.Module);
				enmJsonObj->SetStringField("UnderlyingType", enm.UnderlyingType);
				{
					auto valueMapJsonObj = MakeShared<FJsonObject>();
					for (const auto& pair : enm.ValueMap)
					{
						valueMapJsonObj->SetStringField(pair.Key, pair.Value);
					}
					enmJsonObj->SetObjectField("ValueMap", valueMapJsonObj);
				}

				auto enmJsonValue = MakeShared<FJsonValueObject>(enmJsonObj);
				enums.Emplace(enmJsonValue);
			}

			jsonObj->SetArrayField("Enums", MoveTemp(enums));
		}

		{ // Classes
			TArray<TSharedPtr<FJsonValue>> classes;
			for (const auto& cls : dto.Classes)
			{
				auto clsJsonObj = MakeShared<FJsonObject>();
				clsJsonObj->SetStringField("Name", cls.Name);
				clsJsonObj->SetStringField("Module", cls.Module);
				clsJsonObj->SetStringField("BaseType", cls.BaseType);

				auto clsJsonValue = MakeShared<FJsonValueObject>(clsJsonObj);
				classes.Emplace(clsJsonValue);
			}

			jsonObj->SetArrayField("Classes", MoveTemp(classes));
		}
		
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
	IZExportedTypeRegistry::Get().ForeachExportedEnum([this](const IZExportedEnum& enm){ WriteEnum(enm); });
	IZExportedTypeRegistry::Get().ForeachExportedClass([this](const IZExportedClass& cls){ WriteClass(cls); });
	
	const FString glueDir = FPaths::Combine(FPaths::ProjectDir(), "Intermediate/ZSharp/Glue");
	for (const auto& pair : AssemblyDtoMap)
	{
		TSharedRef<FJsonObject> jsonObj = ZGlueManifestWriter_Private::AssemblyToJson(*pair.Value);
		FString jsonStr;
		if (!ZGlueManifestWriter_Private::JsonToString(jsonObj, jsonStr))
		{
			checkNoEntry();
		}

		FFileHelper::SaveStringToFile(jsonStr, *FPaths::Combine(glueDir, pair.Key, "Manifest.json"));
	}
}

void ZSharp::FZGlueManifestWriter::WriteClass(const IZExportedClass& cls)
{
	TUniquePtr<FZExportedAssemblyDto>* pAssemblyDto = GetAssemblyDto(cls);
	if (!pAssemblyDto)
	{
		return;
	}

	TUniquePtr<FZExportedAssemblyDto>& assemblyDto = *pAssemblyDto;
		
	FZExportedClassDto classDto;
	classDto.Name = cls.GetName();
	classDto.Module = cls.GetModule();
	classDto.BaseType = cls.GetBaseType();
	
	assemblyDto->Classes.Emplace(MoveTemp(classDto));
	
	UE_LOG(LogTemp, Log, TEXT("Exported Class Name: [%s] Module: [%s] BaseType: [%s]"),
		*cls.GetName(),
		*cls.GetModule(),
		*cls.GetBaseType());
}

void ZSharp::FZGlueManifestWriter::WriteEnum(const IZExportedEnum& enm)
{
	TUniquePtr<FZExportedAssemblyDto>* pAssemblyDto = GetAssemblyDto(enm);
	if (!pAssemblyDto)
	{
		return;
	}

	TUniquePtr<FZExportedAssemblyDto>& assemblyDto = *pAssemblyDto;
		
	FZExportedEnumDto enumDto;
	enumDto.Name = enm.GetName();
	enumDto.Module = enm.GetModule();
	enumDto.UnderlyingType = enm.GetUnderlyingType();
	enm.ForeachEnumValue([&enumDto](const FString& name, const FString& value)
	{
		enumDto.ValueMap.Emplace(name, value);
	});
			
	assemblyDto->Enums.Emplace(MoveTemp(enumDto));
	
	UE_LOG(LogTemp, Log, TEXT("Exported Enum Name: [%s] Module: [%s] SlotType: [%s] UnderlyingType: [%s]"),
		*enm.GetName(),
		*enm.GetModule(),
		*ToString(enm.GetSlotType()),
		*enm.GetUnderlyingType());
}

TUniquePtr<ZSharp::FZExportedAssemblyDto>* ZSharp::FZGlueManifestWriter::GetAssemblyDto(const IZExportedType& type)
{
	const UZSharpExportSettings* settings = GetDefault<UZSharpExportSettings>();
	const FString module = type.GetModule();
	FString assembly;
	if (!settings->TryGetModuleAssembly(module, assembly))
	{
		return nullptr;
	}

	TUniquePtr<FZExportedAssemblyDto>& assemblyDto = AssemblyDtoMap.FindOrAdd(assembly);
	if (!assemblyDto)
	{
		assemblyDto = MakeUnique<FZExportedAssemblyDto>();
	}

	return &assemblyDto;
}


