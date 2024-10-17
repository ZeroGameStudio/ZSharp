// Copyright Zero Games. All Rights Reserved.


#include "ZGlueManifestWriter.h"

#include "IZExportedEnum.h"
#include "IZExportedClass.h"
#include "IZExportedDelegate.h"
#include "IZExportedMethod.h"
#include "IZExportedProperty.h"
#include "IZExportedParameter.h"
#include "IZExportedTypeRegistry.h"
#include "ZSharpRuntimeSettings.h"
#include "DTO/ZExportedClassDto.h"
#include "JsonObjectConverter.h"
#include "DTO/ZExportedDelegateDto.h"

void ZSharp::FZGlueManifestWriter::Write(const TArray<FString>& assemblies)
{
	ExplicitAssemblies = assemblies;
	
	IZExportedTypeRegistry::Get().ForeachExportedEnum([this](const IZExportedEnum& enm){ WriteEnum(enm); });
	IZExportedTypeRegistry::Get().ForeachExportedClass([this](const IZExportedClass& cls){ WriteClass(cls); });
	IZExportedTypeRegistry::Get().ForeachExportedDelegate([this](const IZExportedDelegate& delegate){ WriteDelegate(delegate); });
	
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
	classDto.UnrealFieldPath = cls.GetUnrealFieldPath();
	classDto.ConjugateRegistryId = cls.GetConjugateRegistryId();
	classDto.Flags = static_cast<__underlying_type(EZExportedClassFlags)>(cls.GetFlags());
	classDto.BaseType = cls.GetBaseType();
	cls.ForeachMethod([&classDto](const IZExportedMethod& method)
	{
		FZExportedMethodDto methodDto;
		methodDto.Name = method.GetName();
		methodDto.ZCallName = method.GetZCallName();
		methodDto.Flags = static_cast<__underlying_type(EZExportedMethodFlags)>(method.GetFlags());
		method.ForeachParameter([&methodDto](const IZExportedParameter& param)
		{
			FZExportedParameterDto paramDto;
			paramDto.Name = param.GetName();
			paramDto.Type = param.GetType();
			paramDto.UnderlyingType = param.GetUnderlyingType();
			paramDto.Flags = static_cast<__underlying_type(EZExportedParameterFlags)>(param.GetFlags());
			methodDto.Parameters.Emplace(MoveTemp(paramDto));
		});
		classDto.Methods.Emplace(MoveTemp(methodDto));
	});
	cls.ForeachProperty([&classDto](const IZExportedProperty& property)
	{
		FZExportedPropertyDto propertyDto;
		propertyDto.Name = property.GetName();
		propertyDto.Index = property.GetIndex();
		propertyDto.ZCallName = property.GetZCallName();
		propertyDto.Type = property.GetType();
		propertyDto.UnderlyingType = property.GetUnderlyingType();
		propertyDto.Flags = static_cast<__underlying_type(EZExportedPropertyFlags)>(property.GetFlags());
		classDto.Properties.Emplace(MoveTemp(propertyDto));
	});
	
	assemblyDto->Classes.Emplace(MoveTemp(classDto));
	
	UE_LOG(LogTemp, Log, TEXT("Exported Class Name: [%s] Module: [%s] BaseType: [%s]"),
		*cls.GetName(),
		*cls.GetModule(),
		*cls.GetBaseType().Name);
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
	enumDto.UnrealFieldPath = enm.GetUnrealFieldPath();
	enumDto.Flags = static_cast<__underlying_type(EZExportedEnumFlags)>(enm.GetFlags());
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

void ZSharp::FZGlueManifestWriter::WriteDelegate(const IZExportedDelegate& delegate)
{
	TUniquePtr<FZExportedAssemblyDto>* pAssemblyDto = GetAssemblyDto(delegate);
	if (!pAssemblyDto)
	{
		return;
	}

	TUniquePtr<FZExportedAssemblyDto>& assemblyDto = *pAssemblyDto;
		
	FZExportedDelegateDto delegateDto;
	delegateDto.Name = delegate.GetName();
	delegateDto.Module = delegate.GetModule();
	delegateDto.UnrealFieldPath = delegate.GetUnrealFieldPath();
	delegateDto.Flags = static_cast<__underlying_type(EZExportedDelegateFlags)>(delegate.GetFlags());
	delegate.ForeachParameter([&delegateDto](const IZExportedParameter& param)
	{
		FZExportedParameterDto paramDto;
		paramDto.Name = param.GetName();
		paramDto.Type = param.GetType();
		paramDto.UnderlyingType = param.GetUnderlyingType();
		paramDto.Flags = static_cast<__underlying_type(EZExportedParameterFlags)>(param.GetFlags());
		delegateDto.Parameters.Emplace(MoveTemp(paramDto));
	});
			
	assemblyDto->Delegates.Emplace(MoveTemp(delegateDto));
	
	UE_LOG(LogTemp, Log, TEXT("Exported Delegate Name: [%s] Module: [%s]"),
		*delegate.GetName(),
		*delegate.GetModule());
}

TUniquePtr<FZExportedAssemblyDto>* ZSharp::FZGlueManifestWriter::GetAssemblyDto(const IZExportedType& type)
{
	const UZSharpRuntimeSettings* settings = GetDefault<UZSharpRuntimeSettings>();
	const FString module = type.GetModule();
	const FZModuleMappingContext* ctx = settings->GetModuleMappingContext(module);
	if (!ctx)
	{
		return nullptr;
	}

	const FString assembly = ctx->AssemblyName;
	if (!ExplicitAssemblies.IsEmpty() && !ExplicitAssemblies.Contains(assembly))
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


