// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

public static class UnrealFieldDefinitionExtensions
{
	extension(UnrealFieldDefinition @this)
	{
		public void AddMetadata(string key, object value)
		{
			if (string.IsNullOrWhiteSpace(key))
			{
				return;
			}

			@this.MetadataMap ??= new();
			@this.MetadataMap[key] = value is bool ? value.ToString()!.ToLower() : value.ToString() ?? string.Empty;
		}
	
		public void AddMetadata(string key) => AddMetadata(@this, key, string.Empty);

		public void RemoveMetadata(string key)
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
	
		public void AddTransparentData(string key, object value)
		{
			if (string.IsNullOrWhiteSpace(key))
			{
				return;
			}

			@this.TransparentDataMap ??= new();
			@this.TransparentDataMap[key] = value.ToString() ?? string.Empty;
		}
	
		public void AddTransparentData(string key) => AddMetadata(@this, key, string.Empty);

		public void RemoveTransparentData(string key)
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
	}
}


