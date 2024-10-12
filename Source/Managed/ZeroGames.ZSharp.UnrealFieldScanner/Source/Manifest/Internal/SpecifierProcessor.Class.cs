// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

partial class SpecifierProcessor
{
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, UClassAttribute specifier)
	{
		// Nothing to do.
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, WithinAttribute specifier)
	{
		def.WithinPath = ((ITypeModel)specifier.OuterType_SCANTIME).GetUnrealFieldPath();
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, AbstractAttribute specifier)
	{
		def.ClassFlags |= EClassFlags.CLASS_Abstract;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, ConstAttribute specifier)
	{
		def.ClassFlags |= EClassFlags.CLASS_Const;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, OptionalAttribute specifier)
	{
		def.ClassFlags |= EClassFlags.CLASS_Optional;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, DeprecatedAttribute specifier)
	{
		def.ClassFlags |= EClassFlags.CLASS_Deprecated | EClassFlags.CLASS_NotPlaceable;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, ConfigNameAttribute specifier)
	{
		def.ConfigName = specifier.Name;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, PerObjectConfigAttribute specifier)
	{
		def.ClassFlags |= EClassFlags.CLASS_PerObjectConfig;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, PerPlatformConfigAttribute specifier)
	{
		def.ClassFlags |= EClassFlags.CLASS_PerPlatformConfig;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, ConfigDontCheckDefaultsAttribute specifier)
	{
		def.ClassFlags |= EClassFlags.CLASS_ConfigDoNotCheckDefaults;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, DefaultConfigAttribute specifier)
	{
		def.ClassFlags |= EClassFlags.CLASS_DefaultConfig;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, ProjectUserConfigAttribute specifier)
	{
		def.ClassFlags |= EClassFlags.CLASS_ProjectUserConfig;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, GlobalUserConfigAttribute specifier)
	{
		def.ClassFlags |= EClassFlags.CLASS_GlobalUserConfig;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, DefaultToTransientAttribute specifier)
	{
		def.ClassFlags |= EClassFlags.CLASS_Transient;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, DefaultToNonTransientAttribute specifier)
	{
		def.ClassFlags &= ~EClassFlags.CLASS_Transient;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, DefaultToInstancedAttribute specifier)
	{
		def.ClassFlags |= EClassFlags.CLASS_DefaultToInstanced;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, EditInlineNewAttribute specifier)
	{
		def.ClassFlags |= EClassFlags.CLASS_EditInlineNew;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, NotEditInlineNewAttribute specifier)
	{
		def.ClassFlags &= ~EClassFlags.CLASS_EditInlineNew;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, PlaceableAttribute specifier)
	{
		def.ClassFlags &= ~EClassFlags.CLASS_NotPlaceable;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, NotPlaceableAttribute specifier)
	{
		def.ClassFlags |= EClassFlags.CLASS_NotPlaceable;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, HideDropdownAttribute specifier)
	{
		def.ClassFlags |= EClassFlags.CLASS_HideDropDown;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, HiddenAttribute specifier)
	{
		def.ClassFlags |= EClassFlags.CLASS_Hidden;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, CollapseCategoriesAttribute specifier)
	{
		def.ClassFlags |= EClassFlags.CLASS_CollapseCategories;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, DontCollapseCategoriesAttribute specifier)
	{
		def.ClassFlags &= ~EClassFlags.CLASS_CollapseCategories;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, AdvancedClassDisplayAttribute specifier)
	{
		def.AddMetadata(MetadataConstants.AdvancedClassDisplay, true);
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, ConversionRootAttribute specifier)
	{
		def.AddMetadata(MetadataConstants.IsConversionRoot, true);
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, ComponentWrapperAttribute specifier)
	{
		def.AddMetadata(MetadataConstants.IgnoreCategoryKeywordsInSubclasses, true);
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, EditorConfigNameAttribute specifier)
	{
		def.AddMetadata(MetadataConstants.EditorConfig, specifier.Name);
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, ClassGroupAttribute specifier)
	{
		def.AddTransparentData(TransparentDataConstants.ClassGroup, string.Join(' ', specifier.Names));
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, ShowFunctionsAttribute specifier)
	{
		def.AddTransparentData(TransparentDataConstants.ShowFunctions, string.Join(' ', specifier.Names));
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, HideFunctionsAttribute specifier)
	{
		def.AddTransparentData(TransparentDataConstants.HideFunctions, string.Join(' ', specifier.Names));
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, ShowCategoriesAttribute specifier)
	{
		def.AddTransparentData(TransparentDataConstants.ShowCategories, string.Join(' ', specifier.Names));
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, HideCategoriesAttribute specifier)
	{
		def.AddTransparentData(TransparentDataConstants.HideCategories, string.Join(' ', specifier.Names));
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, AutoExpandCategoriesAttribute specifier)
	{
		def.AddTransparentData(TransparentDataConstants.AutoExpandCategories, string.Join(' ', specifier.Names));
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, AutoCollapseCategoriesAttribute specifier)
	{
		def.AddTransparentData(TransparentDataConstants.AutoCollapseCategories, string.Join(' ', specifier.Names));
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, DontAutoCollapseCategoriesAttribute specifier)
	{
		def.AddTransparentData(TransparentDataConstants.DontAutoCollapseCategories, string.Join(' ', specifier.Names));
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, PrioritizeCategoriesAttribute specifier)
	{
		def.AddTransparentData(TransparentDataConstants.PrioritizeCategories, string.Join(' ', specifier.Names));
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


