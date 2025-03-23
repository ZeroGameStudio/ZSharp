// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

partial class ManifestBuilder
{

	private UnrealEnumDefinition ScanEnumModel(IUnrealEnumModel enumModel)
	{
		UnrealEnumDefinition result = new()
		{
			Name = enumModel.Name,
			UnderlyingType = TypeUriToEnumUnderlyingType(enumModel.EnumUnderlyingType),
			EnumFlags = enumModel.IsFlags ? EEnumFlags.Flags : EEnumFlags.None,
		};
		
		ProcessSpecifiers(result, enumModel);

		foreach (var field in enumModel.Fields)
		{
			result.Fields.Add(MakeEnumFieldDefinition(enumModel, field));
		}
		
		return result;
	}

	private UnrealEnumFieldDefinition MakeEnumFieldDefinition(IUnrealEnumModel enumModel, IUnrealEnumFieldModel enumFieldModel)
	{
		UnrealEnumFieldDefinition result = new()
		{
			Name = enumFieldModel.Name,
			Value = enumFieldModel.Value,
		};
		
		ProcessSpecifiers(result, enumFieldModel);
		
		// Migrate from UEnum::GenerateFullEnumName(), ECppForm::EnumClass case.
		result.AddMetadata("Name", $"{enumModel.Name}::{result.Name}");

		return result;
	}

}


