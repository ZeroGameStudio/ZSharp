﻿// Copyright Zero Games. All Rights Reserved.

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

		throw new NotImplementedException();
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

