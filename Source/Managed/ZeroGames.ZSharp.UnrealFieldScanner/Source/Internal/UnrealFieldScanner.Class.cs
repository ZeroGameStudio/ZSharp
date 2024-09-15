// Copyright Zero Games. All Rights Reserved.

using Mono.Cecil;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

partial class UnrealFieldScanner
{

	private void ScanUClass(TypeDefinition type)
	{
		UnrealClassDefinition cls = new()
		{
			Name = type.Name,
			SuperPath = GetUnrealFieldPath(type.BaseType),
		};

		if (GetCustomAttributeOrDefault<WithinAttribute>(type) is { } withinAttr)
		{
			TypeReference outerTypeRef = GetAttributeCtorArgValue<TypeReference>(withinAttr, 0);
			cls.WithinPath = GetUnrealFieldPath(outerTypeRef);
		}

		if (GetCustomAttributeOrDefault<ConfigAttribute>(type) is { } configAttr)
		{
			cls.ConfigName = GetAttributeCtorArgValue<string>(configAttr, 0);
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


