// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

partial class ManifestBuilder
{

	private UnrealEnumDefinition ScanEnumModel(IUnrealEnumModel enumModel)
	{
		UnrealEnumDefinition result = new()
		{
			Name = enumModel.Name,
			EnumFlags = enumModel.IsFlags ? EEnumFlags.Flags : EEnumFlags.None,
		};

		foreach (var field in enumModel.Fields)
		{
			result.Fields.Add(MakeEnumFieldDefinition(field));
		}
		
		return result;
	}

	private UnrealEnumFieldDefinition MakeEnumFieldDefinition(IUnrealEnumFieldModel enumFieldModel)
	{
		UnrealEnumFieldDefinition result = new()
		{
			Name = enumFieldModel.Name,
			Value = enumFieldModel.Value,
		};
		
		ProcessSpecifiers(result, enumFieldModel);

		return result;
	}
	
}


