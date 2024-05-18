// Copyright Zero Games. All Rights Reserved.


#include "ZGlueManifestWriter.h"

#include "IZExportedEnum.h"
#include "IZExportedTypeRegistry.h"
#include "ZSharpExportSettings.h"
#include "DTO/ZExportedClassDto.h"
#include "JsonObjectConverter.h"

void ZSharp::FZGlueManifestWriter::Write()
{
	IZExportedTypeRegistry::Get().ForeachExportedEnum([this](const IZExportedEnum& enm){ WriteEnum(enm); });
	IZExportedTypeRegistry::Get().ForeachExportedClass([this](const IZExportedClass& cls){ WriteClass(cls); });
	
	const FString glueDir = FPaths::Combine(FPaths::ProjectDir(), "Intermediate/ZSharp/Glue");
	for (const auto& pair : AssemblyDtoMap)
	{
		FString jsonStr;
		if (!FJsonObjectConverter::UStructToJsonObjectString<FZExportedAssemblyDto>(*pair.Value, jsonStr))
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
	classDto.Flags = StaticCast<__underlying_type(EZExportedClassFlags)>(cls.GetFlags());
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

TUniquePtr<FZExportedAssemblyDto>* ZSharp::FZGlueManifestWriter::GetAssemblyDto(const IZExportedType& type)
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


