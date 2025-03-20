// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

internal static partial class SpecifierProcessor
{
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFieldDefinition def, IUnrealFieldModel model, UMetaAttribute specifier)
	{
		foreach (var pair in specifier.Pairs)
		{
			string[] kv = pair.Split('=', 2, StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries);
			string key = kv.Length > 0 ? kv[0] : string.Empty;
			string value = kv.Length > 1 ? kv[1] : string.Empty;
			if (!string.IsNullOrWhiteSpace(key))
			{
				def.AddMetadata(key, value);
			}
		}
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFieldDefinition def, IUnrealFieldModel model, BlueprintTypeAttribute specifier)
	{
		def.AddMetadata(MetadataConstants.BlueprintType, true);
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFieldDefinition def, IUnrealFieldModel model, NotBlueprintTypeAttribute specifier)
	{
		def.AddMetadata(MetadataConstants.NotBlueprintType, true);
		def.RemoveMetadata(MetadataConstants.BlueprintType);
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFieldDefinition def, IUnrealFieldModel model, BlueprintableAttribute specifier)
	{
		def.AddMetadata(MetadataConstants.BlueprintType, true);
		def.AddMetadata(MetadataConstants.IsBlueprintBase, true);
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFieldDefinition def, IUnrealFieldModel model, NotBlueprintableAttribute specifier)
	{
		def.RemoveMetadata(MetadataConstants.BlueprintType);
		def.AddMetadata(MetadataConstants.IsBlueprintBase, false);
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFieldDefinition def, IUnrealFieldModel model, BlueprintInternalUseOnlyAttribute specifier)
	{
		def.AddMetadata(MetadataConstants.BlueprintType, true);
		def.AddMetadata(MetadataConstants.BlueprintInternalUseOnly, true);
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFieldDefinition def, IUnrealFieldModel model, BlueprintInternalUseOnlyHierarchicalAttribute specifier)
	{
		def.AddMetadata(MetadataConstants.BlueprintType, true);
		def.AddMetadata(MetadataConstants.BlueprintInternalUseOnly, true);
		def.AddMetadata(MetadataConstants.BlueprintInternalUseOnlyHierarchical, true);
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFieldDefinition def, IUnrealFieldModel model, CategoryAttribute specifier)
	{
		def.AddMetadata(MetadataConstants.Category, specifier.Value);
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFieldDefinition def, IUnrealFieldModel model, DisplayNameAttribute specifier)
	{
		def.AddMetadata(MetadataConstants.DisplayName, specifier.Value);
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFieldDefinition def, IUnrealFieldModel model, FriendlyNameAttribute specifier)
	{
		def.AddMetadata(MetadataConstants.FriendlyName, specifier.Value);
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFieldDefinition def, IUnrealFieldModel model, TooltipAttribute specifier)
	{
		def.AddMetadata(MetadataConstants.Tooltip, specifier.Value);
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFieldDefinition def, IUnrealFieldModel model, ShortTooltipAttribute specifier)
	{
		def.AddMetadata(MetadataConstants.ShortTooltip, specifier.Value);
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFieldDefinition def, IUnrealFieldModel model, ExperimentalAttribute specifier)
	{
		def.AddMetadata(MetadataConstants.DevelopmentStatus, MetadataConstants.Experimental);
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFieldDefinition def, IUnrealFieldModel model, EarlyAccessAttribute specifier)
	{
		def.AddMetadata(MetadataConstants.DevelopmentStatus, MetadataConstants.EarlyAccess);
	}
	
}


