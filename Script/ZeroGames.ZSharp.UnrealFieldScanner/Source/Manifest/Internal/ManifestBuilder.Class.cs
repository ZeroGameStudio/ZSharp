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
			ClassFlags = classModel.IsInternal ? EClassFlags.MinimalAPI : EClassFlags.RequiredAPI,
			HasUClassConstructor = classModel.HasUClassConstructor,
		};
		
		ProcessSpecifiers(result, classModel);
		
		ScanUFunctions(result, classModel);
		ScanUProperties(result, classModel);
		
		foreach (var property in result.Properties)
		{
			if ((property.PropertyFlags & EPropertyFlags.Config) != EPropertyFlags.None)
			{
				result.ClassFlags |= EClassFlags.Config;
				break;
			}
		}
		
		return result;
	}

}


