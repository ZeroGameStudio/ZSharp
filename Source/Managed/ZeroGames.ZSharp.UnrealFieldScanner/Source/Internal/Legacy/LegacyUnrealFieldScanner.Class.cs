// Copyright Zero Games. All Rights Reserved.

using Mono.Cecil;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

partial class LegacyUnrealFieldScanner
{

	private void ScanUClass(TypeDefinition type, CustomAttribute uclassAttr)
	{
		UnrealClassDefinition cls = new()
		{
			Name = type.Name,
			SuperPath = GetUnrealFieldPath(type.BaseType),
		};

		if (TryGetAttributePropertyValue<string>(uclassAttr, nameof(UClassAttribute.Config), out var config))
		{
			cls.ConfigName = config;
		}
		
		if (TryGetAttributePropertyValue<TypeReference>(uclassAttr, nameof(UClassAttribute.Within), out var within))
		{
			cls.WithinPath = GetUnrealFieldPath(within);
		}

		{ // Default
			foreach (var attr in type.CustomAttributes)
			{
				if (attr.AttributeType.FullName == typeof(PropertyDefaultOverrideAttribute).FullName)
				{
					cls.PropertyDefaults.Add(new ()
					{
						PropertyChain = GetAttributePropertyValue<string>(attr, nameof(PropertyDefaultOverrideAttribute.Property)),
						Buffer = GetAttributePropertyValue<object>(attr, nameof(PropertyDefaultOverrideAttribute.Default)).ToString()!,
					});
				}
				else if (attr.AttributeType.FullName == typeof(DefaultSubobjectClassOverrideAttribute).FullName)
				{
					cls.DefaultSubobjectOverrides.Add(new()
					{
						Name = GetAttributePropertyValue<string>(attr, nameof(DefaultSubobjectClassOverrideAttribute.Subobject)),
						ClassPath = "", // @TODO
					});
				}
				else if (attr.AttributeType.FullName == typeof(DontCreateDefaultSubobjectAttribute).FullName)
				{
					cls.DefaultSubobjectOverrides.Add(new()
					{
						Name = GetAttributeCtorArgValue<string>(attr, 0),
					});
				}
			}
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


