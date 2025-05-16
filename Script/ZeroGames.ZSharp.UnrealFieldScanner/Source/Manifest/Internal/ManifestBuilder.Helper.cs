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
	private const string UNREAL_STRING_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "FString";
	private const string UNREAL_UTF8_STRING_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "FUtf8String";
	private const string UNREAL_ANSI_STRING_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "FAnsiString";
	private const string UNREAL_NAME_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "FName";
	private const string UNREAL_TEXT_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "FText";
	
	// Object wrappers
	private const string SUBCLASS_OF_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "TSubclassOf`1";
	private const string SOFT_CLASS_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "TSoftClassPtr`1";
	private const string SOFT_OBJECT_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "TSoftObjectPtr`1";
	private const string WEAK_OBJECT_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "TWeakObjectPtr`1";
	private const string LAZY_OBJECT_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "TLazyObjectPtr`1";
	private const string SCRIPT_INTERFACE_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "TScriptInterface`1";
	
	// Containers
	private const string UNREAL_ARRAY_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "TArray`1";
	private const string UNREAL_SET_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "TSet`1";
	private const string UNREAL_MAP_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "TMap`2";
	private const string UNREAL_OPTIONAL_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "TOptional`1";
	
	// Delegates
	private const string UNREAL_DELEGATE_BASE_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "UnrealDelegateBase";
	private const string UNREAL_MULTICAST_INLINE_DELEGATE_BASE_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "UnrealMulticastInlineDelegateBase";
	private const string UNREAL_MULTICAST_SPARSE_DELEGATE_BASE_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "UnrealMulticastSparseDelegateBase";
	
	// Special types
	private const string FIELD_PATH_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "FFieldPath";
	
	// Traits
	private const string STATIC_CLASS_INTERFACE_FULL_NAME = ENGINE_NAMESPACE + "IStaticClass";
	private const string STATIC_STRUCT_INTERFACE_FULL_NAME = ENGINE_NAMESPACE + "IStaticStruct";
	private const string STATIC_SIGNATURE_INTERFACE_FULL_NAME = ENGINE_NAMESPACE + "IStaticSignature";
	
}


