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
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, ContextualAttribute specifier)
	{
		def.IsContextual = true;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, WithinAttribute specifier)
	{
		def.WithinPath = ((ITypeModel)specifier.OuterType_SCANTIME).UnrealFieldPath;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, AbstractAttribute specifier)
	{
		def.ClassFlags |= EClassFlags.Abstract;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, ConstAttribute specifier)
	{
		throw new NotImplementedException();
		
		// def.ClassFlags |= EClassFlags.CLASS_Const;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, OptionalAttribute specifier)
	{
		def.ClassFlags |= EClassFlags.Optional;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, DeprecatedAttribute specifier)
	{
		def.ClassFlags |= EClassFlags.Deprecated | EClassFlags.NotPlaceable;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, ConfigNameAttribute specifier)
	{
		def.ConfigName = specifier.Name;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, PerObjectConfigAttribute specifier)
	{
		def.ClassFlags |= EClassFlags.PerObjectConfig;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, PerPlatformConfigAttribute specifier)
	{
		def.ClassFlags |= EClassFlags.PerPlatformConfig;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, ConfigDontCheckDefaultsAttribute specifier)
	{
		def.ClassFlags |= EClassFlags.ConfigDoNotCheckDefaults;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, DefaultConfigAttribute specifier)
	{
		def.ClassFlags |= EClassFlags.DefaultConfig;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, ProjectUserConfigAttribute specifier)
	{
		def.ClassFlags |= EClassFlags.ProjectUserConfig;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, GlobalUserConfigAttribute specifier)
	{
		def.ClassFlags |= EClassFlags.GlobalUserConfig;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, DefaultToTransientAttribute specifier)
	{
		def.ClassFlags |= EClassFlags.Transient;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, DefaultToNonTransientAttribute specifier)
	{
		def.ClassFlags &= ~EClassFlags.Transient;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, DefaultToInstancedAttribute specifier)
	{
		def.ClassFlags |= EClassFlags.DefaultToInstanced;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, EditInlineNewAttribute specifier)
	{
		def.ClassFlags |= EClassFlags.EditInlineNew;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, NotEditInlineNewAttribute specifier)
	{
		def.ClassFlags &= ~EClassFlags.EditInlineNew;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, PlaceableAttribute specifier)
	{
		def.ClassFlags &= ~EClassFlags.NotPlaceable;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, NotPlaceableAttribute specifier)
	{
		def.ClassFlags |= EClassFlags.NotPlaceable;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, HideDropdownAttribute specifier)
	{
		def.ClassFlags |= EClassFlags.HideDropDown;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, HiddenAttribute specifier)
	{
		def.ClassFlags |= EClassFlags.Hidden;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, CollapseCategoriesAttribute specifier)
	{
		def.ClassFlags |= EClassFlags.CollapseCategories;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, DontCollapseCategoriesAttribute specifier)
	{
		def.ClassFlags &= ~EClassFlags.CollapseCategories;
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
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, DefaultSubobjectClassOverrideAttribute specifier)
	{
		def.DefaultSubobjectOverrides.Add(new()
		{
			Name = specifier.Subobject,
			ClassPath = ((ITypeModel)specifier.Class_SCANTIME).UnrealFieldPath,
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
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, DefaultToReplicatedAttribute specifier)
	{
		def.IsDefaultToReplicated = true;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealClassDefinition def, IUnrealClassModel model, BlueprintSpawnableComponentAttribute specifier)
	{
		def.AddMetadata(MetadataConstants.BlueprintSpawnableComponent, true);
	}
	
}


