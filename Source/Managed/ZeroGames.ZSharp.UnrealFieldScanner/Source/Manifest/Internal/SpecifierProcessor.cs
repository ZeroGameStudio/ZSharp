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
				def.MetadataMap[key] = value;
			}
		}
	}
	
}


