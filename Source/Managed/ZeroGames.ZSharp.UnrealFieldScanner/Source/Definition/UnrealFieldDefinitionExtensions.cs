// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

public static class UnrealFieldDefinitionExtensions
{
	
	public static void AddMetadata(this UnrealFieldDefinition @this, string key, object value)
	{
		if (string.IsNullOrWhiteSpace(key))
		{
			return;
		}
		
		@this.MetadataMap[key] = value.ToString() ?? string.Empty;
	}
	
	public static void AddMetadata(this UnrealFieldDefinition @this, string key) => AddMetadata(@this, key, string.Empty);

	public static void RemoveMetadata(this UnrealFieldDefinition @this, string key)
	{
		if (string.IsNullOrWhiteSpace(key))
		{
			return;
		}
		
		@this.MetadataMap.Remove(key);
	}
	
}


