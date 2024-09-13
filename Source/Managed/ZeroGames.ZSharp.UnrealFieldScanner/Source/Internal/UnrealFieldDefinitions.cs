// Copyright Zero Games. All Rights Reserved.

using System.Text.Json.Serialization;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

public class UnrealFieldDefinition
{
	public required string Name { get; set; }
	public EObjectFlags Flags { get; set; }
	public Dictionary<string, string> MetadataMap { get; set; } = new();
}

// IMPORTANT: KEEP SYNC WITH ZUnrealFieldDefinitions.h
public enum EPropertyType : uint8
{
	None,
	// Primitives
	UInt8, UInt16, UInt32, UInt64, Int8, Int16, Int32, Int64, Float, Double, Bool, Enum,
	// Strings
	String, Name, Text,
	// Object wrappers
	Object, Class, SoftClass, SoftObject, WeakObject, LazyObject, Interface,
	// Containers
	Struct, Array, Set, Map, Optional,
	// Delegates
	Delegate, MulticastInlineDelegate, // MulticastSparseDelegate is not supported.
	// Special types
	FieldPath, // VerseValue is not supported.
};

public class UnrealPropertyDefinition : UnrealFieldDefinition
{
	public required EPropertyType Type { get; set; }
	public EPropertyFlags PropertyFlags { get; set; }
	public string? RepNotifyName { get; set; }
	public string? DescriptorFieldPath { get; set; }

	public UnrealPropertyDefinition? InnerProperty { get; set; }
	public UnrealPropertyDefinition? OuterProperty { get; set; }
}

public class UnrealStructDefinition : UnrealFieldDefinition
{
	public string? SuperPath { get; set; }
	public Dictionary<string, UnrealPropertyDefinition> PropertyMap { get; set; } = new();
}

public class UnrealFunctionDefinition : UnrealStructDefinition
{
	public EFunctionFlags FunctionFlags { get; set; }
	public uint16 RpcId { get; set; }
	public uint16 RpcResponseId { get; set; }
}

public class UnrealEnumDefinition : UnrealFieldDefinition
{
	// @TODO
}

public class UnrealScriptStructDefinition : UnrealStructDefinition
{
	// @TODO
}

public class UnrealClassDefinition : UnrealStructDefinition
{
	public string? ConfigName { get; set; }
	public string? WithinPath { get; set; }
	public EClassFlags ClassFlags { get; set; }
	public EClassCastFlags CastFlags { get; set; }
	public List<string> ImplementedInterfacePaths { get; set; } = new();
	public Dictionary<string, UnrealFunctionDefinition> FunctionMap { get; set; } = new();
}

public class UnrealInterfaceDefinition : UnrealStructDefinition
{
	// @TODO
}

public class UnrealDelegateDefinition : UnrealStructDefinition
{
	// @TODO
}

public class UnrealFieldManifest
{
	public required string ModuleName { get; set; }
	public Dictionary<string, UnrealEnumDefinition> EnumMap { get; set; } = new();
	public Dictionary<string, UnrealScriptStructDefinition> StructMap { get; set; } = new();
	public Dictionary<string, UnrealClassDefinition> ClassMap { get; set; } = new();
	public Dictionary<string, UnrealInterfaceDefinition> InterfaceMap { get; set; } = new();
	public Dictionary<string, UnrealDelegateDefinition> DelegateMap { get; set; } = new();

	[JsonIgnore]
	public bool IsEmpty => EnumMap.Count + StructMap.Count + ClassMap.Count + InterfaceMap.Count + DelegateMap.Count == 0;
}


