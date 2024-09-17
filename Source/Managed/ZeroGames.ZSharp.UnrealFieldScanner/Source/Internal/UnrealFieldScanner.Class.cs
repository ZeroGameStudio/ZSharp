// Copyright Zero Games. All Rights Reserved.

using Mono.Cecil;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

partial class UnrealFieldScanner
{

	private void ScanUClass(TypeDefinition type, CustomAttribute uclassAttr)
	{
		UnrealClassDefinition cls = new()
		{
			Name = type.Name,
			SuperPath = GetUnrealFieldPath(type.BaseType),
		};

		if (TryGetAttributePropertyValue<string?>(uclassAttr, nameof(UClassAttribute.Config), out var config) && config is not null)
		{
			cls.ConfigName = config;
		}
		
		if (TryGetAttributePropertyValue<TypeReference?>(uclassAttr, nameof(UClassAttribute.Within), out var within) && within is not null)
		{
			cls.WithinPath = GetUnrealFieldPath(within);
		}

		ScanUFunctions(type, cls);
		ScanUProperties(type, cls);
		
		if (_withMetadata)
		{
			ScanMetadata(type, cls);
			cls.MetadataMap["IsBlueprintBase"] = "true";
			cls.MetadataMap["BlueprintType"] = "true";
		}
		
		lock (_manifest.Classes)
		{
			_manifest.Classes.Add(cls);
		}
	}
	
}


