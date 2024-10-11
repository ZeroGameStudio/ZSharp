// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

internal static partial class SpecifierProcessor
{

	private static void AddMetadata(UnrealFieldDefinition def, string key, object value)
	{
		if (string.IsNullOrWhiteSpace(key))
		{
			return;
		}
		
		def.MetadataMap[key] = value.ToString() ?? string.Empty;
	}
	
	private static void AddMetadata(UnrealFieldDefinition def, string key) => AddMetadata(def, key, string.Empty);

	private static void RemoveMetadata(UnrealFieldDefinition def, string key)
	{
		if (string.IsNullOrWhiteSpace(key))
		{
			return;
		}
		
		def.MetadataMap.Remove(key);
	}
	
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
				AddMetadata(def, key, value);
			}
		}
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFieldDefinition def, IUnrealFieldModel model, BlueprintTypeAttribute specifier)
	{
		AddMetadata(def, MetadataConstants.BlueprintType, true);
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFieldDefinition def, IUnrealFieldModel model, NotBlueprintTypeAttribute specifier)
	{
		AddMetadata(def, MetadataConstants.NotBlueprintType, true);
		RemoveMetadata(def, MetadataConstants.BlueprintType);
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFieldDefinition def, IUnrealFieldModel model, BlueprintableAttribute specifier)
	{
		AddMetadata(def, MetadataConstants.BlueprintType, true);
		AddMetadata(def, MetadataConstants.IsBlueprintBase, true);
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFieldDefinition def, IUnrealFieldModel model, NotBlueprintableAttribute specifier)
	{
		RemoveMetadata(def, MetadataConstants.BlueprintType);
		AddMetadata(def, MetadataConstants.IsBlueprintBase, false);
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFieldDefinition def, IUnrealFieldModel model, BlueprintInternalUseOnlyAttribute specifier)
	{
		AddMetadata(def, MetadataConstants.BlueprintType, true);
		AddMetadata(def, MetadataConstants.BlueprintInternalUseOnly, true);
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFieldDefinition def, IUnrealFieldModel model, BlueprintInternalUseOnlyHierarchicalAttribute specifier)
	{
		AddMetadata(def, MetadataConstants.BlueprintType, true);
		AddMetadata(def, MetadataConstants.BlueprintInternalUseOnly, true);
		AddMetadata(def, MetadataConstants.BlueprintInternalUseOnlyHierarchical, true);
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFieldDefinition def, IUnrealFieldModel model, CategoryAttribute specifier)
	{
		AddMetadata(def, MetadataConstants.Category, specifier.Value);
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFieldDefinition def, IUnrealFieldModel model, DisplayNameAttribute specifier)
	{
		AddMetadata(def, MetadataConstants.DisplayName, specifier.Value);
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFieldDefinition def, IUnrealFieldModel model, FriendlyNameAttribute specifier)
	{
		AddMetadata(def, MetadataConstants.FriendlyName, specifier.Value);
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFieldDefinition def, IUnrealFieldModel model, TooltipAttribute specifier)
	{
		AddMetadata(def, MetadataConstants.Tooltip, specifier.Value);
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFieldDefinition def, IUnrealFieldModel model, ShortTooltipAttribute specifier)
	{
		AddMetadata(def, MetadataConstants.ShortTooltip, specifier.Value);
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFieldDefinition def, IUnrealFieldModel model, ExperimentalAttribute specifier)
	{
		AddMetadata(def, MetadataConstants.DevelopmentStatus, MetadataConstants.Experimental);
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFieldDefinition def, IUnrealFieldModel model, EarlyAccessAttribute specifier)
	{
		AddMetadata(def, MetadataConstants.DevelopmentStatus, MetadataConstants.EarlyAccess);
	}
	
}


