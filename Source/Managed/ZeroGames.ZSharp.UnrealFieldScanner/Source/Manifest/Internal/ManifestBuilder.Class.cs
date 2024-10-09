// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

partial class ManifestBuilder
{

	private UnrealClassDefinition ScanClassModel(IUnrealClassModel classModel)
	{
		UnrealClassDefinition result = new()
		{
			Name = classModel.Name,
			SuperPath = classModel.BaseType?.GetUnrealFieldPath(),
		};

		ProcessSpecifiers(result, classModel);
		
		ScanUFunctions(result, classModel);
		ScanUProperties(result, classModel);
		
		// @FIXME
		result.MetadataMap["IsBlueprintBase"] = "true";
		result.MetadataMap["BlueprintType"] = "true";

		return result;
	}

}


