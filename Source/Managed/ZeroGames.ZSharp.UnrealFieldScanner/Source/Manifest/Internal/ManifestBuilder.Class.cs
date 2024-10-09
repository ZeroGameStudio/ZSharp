// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

partial class ManifestBuilder
{

	private UnrealClassDefinition ScanClassModel(IUnrealClassModel classModel)
	{
		UnrealClassDefinition result = new()
		{
			Name = classModel.Name,
			SuperPath = GetUnrealFieldPath(classModel.BaseType!),
		};

		ProcessSpecifiers(result, classModel);
		
		ScanUFunctions(result, classModel);
		ScanUProperties(result, classModel);
		
		// @FIXME
		result.MetadataMap["IsBlueprintBase"] = "true";
		result.MetadataMap["BlueprintType"] = "true";

		return result;
	}

	[SpecifierProcessor]
	private void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, UClassAttribute specifier)
	{
		def.ConfigName = specifier.Config;
		if (specifier.Within_SCANTIME is ITypeModel within)
		{
			def.WithinPath = GetUnrealFieldPath(within);
		}
	}
	
	[SpecifierProcessor]
	private void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, PropertyDefaultOverrideAttribute specifier)
	{
		def.PropertyDefaults.Add(new()
		{
			PropertyChain = specifier.Property,
			Buffer = specifier.Default.ToString()!,
		});
	}
	
	[SpecifierProcessor]
	private void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, DefaultSubobjectClassOverrideAttribute specifier)
	{
		def.DefaultSubobjectOverrides.Add(new()
		{
			Name = specifier.Subobject,
			ClassPath = GetUnrealFieldPath((ITypeModel)specifier.Class_SCANTIME),
		});
	}
	
	[SpecifierProcessor]
	private void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, DontCreateDefaultSubobjectAttribute specifier)
	{
		def.DefaultSubobjectOverrides.Add(new()
		{
			Name = specifier.Subobject,
		});
	}

}


