// Copyright Zero Games. All Rights Reserved.

using Mono.Cecil;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

partial class LegacyUnrealFieldScanner
{

	private void ScanMetadata(ICustomAttributeProvider provider, UnrealFieldDefinition field)
	{
		if (GetMetadataMap(provider) is { } metadataMap)
		{
			foreach (var pair in metadataMap)
			{
				field.MetadataMap[pair.Key] = pair.Value;
			}
		}
	}
	
}


