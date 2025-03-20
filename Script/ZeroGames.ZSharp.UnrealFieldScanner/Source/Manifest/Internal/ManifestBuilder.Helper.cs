// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

partial class ManifestBuilder
{
	
	private static EEnumUnderlyingType TypeUriToEnumUnderlyingType(ITypeUri type)
		=> type.FullName switch
		{
			"System.Byte" => EEnumUnderlyingType.UInt8,
			"System.UInt16" => EEnumUnderlyingType.UInt16,
			"System.UInt32" => EEnumUnderlyingType.UInt32,
			"System.UInt64" => EEnumUnderlyingType.UInt64,
			"System.SByte" => EEnumUnderlyingType.Int8,
			"System.Int16" => EEnumUnderlyingType.Int16,
			"System.Int32" => EEnumUnderlyingType.Int32,
			"System.Int64" => EEnumUnderlyingType.Int64,
			_ => throw new ArgumentOutOfRangeException(nameof(type)),
		};
	
		// Namespaces
	private const string ZSHARP_NAMESPACE = "ZeroGames.ZSharp.";
	private const string ENGINE_NAMESPACE = ZSHARP_NAMESPACE + "UnrealEngine.";
	private const string ENGINE_CORE_UOBJECT_NAMESPACE = ENGINE_NAMESPACE + "CoreUObject.";
	
	// Strings
	private const string UNREAL_STRING_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "UnrealString";
	private const string UNREAL_UTF8_STRING_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "UnrealUtf8String";
	private const string UNREAL_ANSI_STRING_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "UnrealAnsiString";
	private const string UNREAL_NAME_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "UnrealName";
	private const string UNREAL_TEXT_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "UnrealText";
	
	// Object wrappers
	private const string SUBCLASS_OF_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "SubclassOf`1";
	private const string SOFT_CLASS_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "SoftClassPtr`1";
	private const string SOFT_OBJECT_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "SoftObjectPtr`1";
	private const string WEAK_OBJECT_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "WeakObjectPtr`1";
	private const string LAZY_OBJECT_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "LazyObjectPtr`1";
	private const string SCRIPT_INTERFACE_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "ScriptInterface`1";
	
	// Containers
	private const string UNREAL_ARRAY_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "UnrealArray`1";
	private const string UNREAL_SET_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "UnrealSet`1";
	private const string UNREAL_MAP_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "UnrealMap`2";
	private const string UNREAL_OPTIONAL_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "UnrealOptional`1";
	
	// Delegates
	private const string UNREAL_DELEGATE_BASE_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "UnrealDelegateBase";
	private const string UNREAL_MULTICAST_INLINE_DELEGATE_BASE_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "UnrealMulticastInlineDelegateBase";
	private const string UNREAL_MULTICAST_SPARSE_DELEGATE_BASE_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "UnrealMulticastSparseDelegateBase";
	
	// Special types
	private const string FIELD_PATH_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "FieldPath";
	
	// Traits
	private const string UNREAL_CLASS_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "UnrealClass";
	private const string STATIC_CLASS_INTERFACE_FULL_NAME = ENGINE_NAMESPACE + "IStaticClass";
	private const string STATIC_STRUCT_INTERFACE_FULL_NAME = ENGINE_NAMESPACE + "IStaticStruct";
	private const string STATIC_SIGNATURE_INTERFACE_FULL_NAME = ENGINE_NAMESPACE + "IStaticSignature";
	
}


