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

		@this.MetadataMap ??= new();
		@this.MetadataMap[key] = ToString(value);
	}
	
	public static void AddMetadata(this UnrealFieldDefinition @this, string key) => AddMetadata(@this, key, string.Empty);

	public static void RemoveMetadata(this UnrealFieldDefinition @this, string key)
	{
		if (@this.MetadataMap is null)
		{
			return;
		}
		
		if (string.IsNullOrWhiteSpace(key))
		{
			return;
		}
		
		@this.MetadataMap.Remove(key);
	}
	
	public static void AddTransparentData(this UnrealFieldDefinition @this, string key, object value)
	{
		if (string.IsNullOrWhiteSpace(key))
		{
			return;
		}

		@this.TransparentDataMap ??= new();
		@this.TransparentDataMap[key] = value.ToString() ?? string.Empty;
	}
	
	public static void AddTransparentData(this UnrealFieldDefinition @this, string key) => AddMetadata(@this, key, string.Empty);

	public static void RemoveTransparentData(this UnrealFieldDefinition @this, string key)
	{
		if (@this.TransparentDataMap is null)
		{
			return;
		}
		
		if (string.IsNullOrWhiteSpace(key))
		{
			return;
		}
		
		@this.TransparentDataMap.Remove(key);
	}

	private static string ToString(object value)
	{
		if (value is bool)
		{
			return value.ToString()!.ToLower();
		}
		
		return value.ToString() ?? string.Empty;
	}
	
}


