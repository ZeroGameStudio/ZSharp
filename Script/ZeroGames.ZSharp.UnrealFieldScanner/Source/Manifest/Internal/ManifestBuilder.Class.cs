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
			ClassFlags = classModel.IsInternal ? EClassFlags.CLASS_MinimalAPI : EClassFlags.CLASS_RequiredAPI,
			HasUClassConstructor = classModel.HasUClassConstructor,
		};
		
		ProcessSpecifiers(result, classModel);
		
		ScanUFunctions(result, classModel);
		ScanUProperties(result, classModel);
		
		foreach (var property in result.Properties)
		{
			if ((property.PropertyFlags & EPropertyFlags.CPF_Config) != EPropertyFlags.CPF_None)
			{
				result.ClassFlags |= EClassFlags.CLASS_Config;
				break;
			}
		}
		
		return result;
	}

}


