// Copyright Zero Games. All Rights Reserved.

using Mono.Cecil;
using ZeroGames.ZSharp.Emit.Specifier.Property;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

partial class UnrealFieldScanner
{

	private void ScanUProperties(TypeDefinition type, UnrealStructDefinition strct)
	{
		foreach (var prop in type.Properties.Where(prop => GetCustomAttributeOrDefault(prop, typeof(UPropertyAttribute).FullName!) is not null))
		{
			ScanUProperty(type, strct, prop);
		}
	}

	private void ScanUProperty(TypeDefinition type, UnrealStructDefinition strct, PropertyDefinition prop)
	{
		UnrealPropertyDefinition def = new()
		{
			Name = prop.Name,
			Type = GetPropertyType(prop.PropertyType, out var descriptorFieldPath),
			DescriptorFieldPath = descriptorFieldPath,
		};

		def.PropertyFlags |= EPropertyFlags.CPF_NativeAccessSpecifierPublic;
		def.PropertyFlags |= EPropertyFlags.CPF_BlueprintVisible;

		if (_withMetadata)
		{
			ScanMetadata(prop, def);
		}

		strct.Properties.Add(def);
	}

	private void ScanUParams(MethodDefinition method, UnrealFunctionDefinition function)
	{
		
	}

	private EPropertyType GetPropertyType(TypeReference propertyTypeRef, out string? descriptorFieldPath)
	{
		descriptorFieldPath = null;
		
		EPropertyType type = propertyTypeRef.FullName switch
		{
			// Primitives
			UINT8_TYPE_FULL_NAME => EPropertyType.UInt8,
			UINT16_TYPE_FULL_NAME => EPropertyType.UInt16,
			UINT32_TYPE_FULL_NAME => EPropertyType.UInt32,
			UINT64_TYPE_FULL_NAME => EPropertyType.UInt64,
			INT8_TYPE_FULL_NAME => EPropertyType.Int8,
			INT16_TYPE_FULL_NAME => EPropertyType.Int16,
			INT32_TYPE_FULL_NAME => EPropertyType.Int32,
			INT64_TYPE_FULL_NAME => EPropertyType.Int64,
			FLOAT_TYPE_FULL_NAME => EPropertyType.Float,
			DOUBLE_TYPE_FULL_NAME => EPropertyType.Double,
			BOOL_TYPE_FULL_NAME => EPropertyType.Bool,
			// Strings
			STRING_TYPE_FULL_NAME => EPropertyType.String, // Special support for System.String.
			UNREAL_STRING_TYPE_FULL_NAME => EPropertyType.String,
			UNREAL_NAME_TYPE_FULL_NAME => EPropertyType.Name,
			UNREAL_TEXT_TYPE_FULL_NAME => EPropertyType.Text,
			// Special types
			FIELD_PATH_TYPE_FULL_NAME => EPropertyType.FieldPath,
			_ => default
		};

		if (type != default)
		{
			return type;
		}

		TypeDefinition propertyType = GetTypeDefinition(propertyTypeRef);
		TypeReference propertyTypeSuperRef = propertyType.BaseType;
		if (propertyTypeSuperRef.FullName == ENUM_TYPE_FULL_NAME && GetUnrealFieldPathOrDefault(propertyType) is {} unrealFieldPath)
		{
			descriptorFieldPath = unrealFieldPath;
			return EPropertyType.Enum;
		}
		
		// @TODO: Object wrappers, containers, delegates

		if (type == default)
		{
			throw new NotSupportedException($"Property type [{propertyTypeRef.FullName}] is not supported.");
		}

		return type;
	}

	private const string SYSTEM_NAMESPACE = "System.";
	private const string ENGINE_NAMESPACE = "ZeroGames.ZSharp.UnrealEngine.";
	private const string ENGINE_CORE_NAMESPACE = ENGINE_NAMESPACE + "Core.";
	private const string ENGINE_CORE_UOBJECT_NAMESPACE = ENGINE_NAMESPACE + "CoreUObject.";

	// Primitives
	private const string UINT8_TYPE_FULL_NAME = SYSTEM_NAMESPACE + nameof(Byte);
	private const string UINT16_TYPE_FULL_NAME = SYSTEM_NAMESPACE + nameof(UInt16);
	private const string UINT32_TYPE_FULL_NAME = SYSTEM_NAMESPACE + nameof(UInt32);
	private const string UINT64_TYPE_FULL_NAME = SYSTEM_NAMESPACE + nameof(UInt64);
	private const string INT8_TYPE_FULL_NAME = SYSTEM_NAMESPACE + nameof(SByte);
	private const string INT16_TYPE_FULL_NAME = SYSTEM_NAMESPACE + nameof(Int16);
	private const string INT32_TYPE_FULL_NAME = SYSTEM_NAMESPACE + nameof(Int32);
	private const string INT64_TYPE_FULL_NAME = SYSTEM_NAMESPACE + nameof(Int64);
	private const string FLOAT_TYPE_FULL_NAME = SYSTEM_NAMESPACE + nameof(Single);
	private const string DOUBLE_TYPE_FULL_NAME = SYSTEM_NAMESPACE + nameof(Double);
	private const string BOOL_TYPE_FULL_NAME = SYSTEM_NAMESPACE + nameof(Boolean);
	private const string ENUM_TYPE_FULL_NAME = SYSTEM_NAMESPACE + nameof(Enum);

	// Strings
	private const string STRING_TYPE_FULL_NAME = SYSTEM_NAMESPACE + nameof(String);
	private const string UNREAL_STRING_TYPE_FULL_NAME = ENGINE_CORE_NAMESPACE + "ZeroGames.ZSharp.UnrealEngine.Core.UnrealString";
	private const string UNREAL_NAME_TYPE_FULL_NAME = ENGINE_CORE_NAMESPACE + "ZeroGames.ZSharp.UnrealEngine.Core.UnrealName";
	private const string UNREAL_TEXT_TYPE_FULL_NAME = ENGINE_CORE_NAMESPACE + "ZeroGames.ZSharp.UnrealEngine.Core.UnrealText";
	
	// Object wrappers
	private const string UNREAL_OBJECT_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "UnrealObjectBase";
	private const string SUBCLASSOF_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "SubclassOf`1";
	private const string SOFT_CLASS_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "SoftClassPtr`1";
	private const string SOFT_OBJECT_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "SoftObjectPtr`1";
	private const string WEAK_OBJECT_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "WeakObjectPtr`1";
	private const string LAZY_OBJECT_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "LazyObjectPtr`1";
	private const string UNREAL_INTERFACE_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "ScriptInterface`1";
	
	// Containers
	private const string UNREAL_SCRIPT_STRUCT_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "UnrealStructBase";
	private const string UNREAL_ARRAY_TYPE_FULL_NAME = ENGINE_CORE_NAMESPACE + "UnrealArray`1";
	private const string UNREAL_SET_TYPE_FULL_NAME = ENGINE_CORE_NAMESPACE + "UnrealSet`1";
	private const string UNREAL_MAP_TYPE_FULL_NAME = ENGINE_CORE_NAMESPACE + "UnrealMap`2";
	private const string UNREAL_OPTIONAL_TYPE_FULL_NAME = ENGINE_CORE_NAMESPACE + "UnrealOptional`1";
	
	// Delegates
	private const string UNREAL_DELEGATE_TYPE_FULL_NAME = ENGINE_CORE_NAMESPACE + "UnrealDelegateBase";
	private const string UNREAL_MULTICAST_INLINE_DELEGATE_TYPE_FULL_NAME = ENGINE_CORE_NAMESPACE + "UnrealMulticastInlineDelegateBase";
	
	// Special types
	private const string FIELD_PATH_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "FieldPath";
	

}


