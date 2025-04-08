// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

partial class ManifestBuilder
{

	private UnrealScriptStructDefinition ScanScriptStructModel(IUnrealScriptStructModel scriptStructModel)
	{
		UnrealScriptStructDefinition result = new()
		{
			Name = scriptStructModel.Name,
			SuperPath = scriptStructModel.BaseType?.GetUnrealFieldPath(),
			StructFlags = EStructFlags.NoFlags,
			HasNetSerialize = scriptStructModel.HasNetSerialize,
		};

		ProcessSpecifiers(result, scriptStructModel);
		
		ScanUProperties(result, scriptStructModel);

		return result;
	}
	
}


