// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

partial class SpecifierProcessor
{
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, UClassAttribute specifier)
	{
		def.ConfigName = specifier.Config;
		if (specifier.Within_SCANTIME is ITypeModel within)
		{
			def.WithinPath = within.GetUnrealFieldPath();
		}
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, PropertyDefaultOverrideAttribute specifier)
	{
		def.PropertyDefaults.Add(new()
		{
			PropertyChain = specifier.Property,
			Buffer = specifier.Default.ToString()!,
		});
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, DefaultSubobjectClassOverrideAttribute specifier)
	{
		def.DefaultSubobjectOverrides.Add(new()
		{
			Name = specifier.Subobject,
			ClassPath = ((ITypeModel)specifier.Class_SCANTIME).GetUnrealFieldPath(),
		});
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, DontCreateDefaultSubobjectAttribute specifier)
	{
		def.DefaultSubobjectOverrides.Add(new()
		{
			Name = specifier.Subobject,
		});
	}
	
}


