// Copyright Zero Games. All Rights Reserved.

using System.Text.Json.Serialization;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

public abstract class UnrealFieldDefinition
{
	public virtual string GetDisplayName()
	{
		return Name;
	}
	
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

public interface ISimpleUnrealPropertyDefinition
{
	EPropertyType Type { get; set; }
	EPropertyFlags PropertyFlags { get; set; }
	string? DescriptorFieldPath { get; set; }
}

public class SimpleUnrealPropertyDefinition : ISimpleUnrealPropertyDefinition
{
	public required EPropertyType Type { get; set; }
	public EPropertyFlags PropertyFlags { get; set; }
	public string? DescriptorFieldPath { get; set; }
}

public class UnrealPropertyDefinition : UnrealFieldDefinition, ISimpleUnrealPropertyDefinition
{
	public override string GetDisplayName()
	{
		return $"{Outer.GetDisplayName()}.{base.GetDisplayName()}";
	}

	[JsonIgnore] public UnrealStructDefinition Outer { get; init; } = null!; // Can't use required for JsonIgnore property...
	public required EPropertyType Type { get; set; }
	public EPropertyFlags PropertyFlags { get; set; }
	public string? DescriptorFieldPath { get; set; }
	
	public string? RepNotifyName { get; set; }

	public SimpleUnrealPropertyDefinition? InnerProperty { get; set; }
	public SimpleUnrealPropertyDefinition? OuterProperty { get; set; }
}

public abstract class UnrealStructDefinition : UnrealFieldDefinition
{
	public string? SuperPath { get; set; }
	public List<UnrealPropertyDefinition> Properties { get; set; } = new();
}

public class UnrealFunctionDefinition : UnrealStructDefinition
{
	public override string GetDisplayName()
	{
		return $"{Outer.GetDisplayName()}.{base.GetDisplayName()}";
	}
	
	[JsonIgnore] public UnrealClassDefinition Outer { get; init; } = null!; // Can't use required for JsonIgnore property...
	public EFunctionFlags FunctionFlags { get; set; }
	public required string ZCallName { get; set; }
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
	public struct PropertyDefault
	{
		public required string PropertyChain { get; set; }
		public required string Buffer { get; set; }
	}

	public struct DefaultSubobject
	{
		public required string Name { get; set; }
		public required string ClassPath { get; set; }
		[JsonPropertyName("bOptional")] public bool IsOptional { get; set; }
		[JsonPropertyName("bTransient")] public bool IsTransient { get; set; }
		public string? PropertyName { get; set; }
		[JsonPropertyName("bRootComponent")] public bool IsRootComponent { get; set; }
		public string? AttachParentDefaultSubobjectName { get; set; }
		public string? AttachSocketName { get; set; }
	}

	public struct DefaultSubobjectOverride
	{
		public required string Name { get; set; }
		public string? ClassPath { get; set; }
	}
	
	public string? ConfigName { get; set; }
	public string? WithinPath { get; set; }
	public EClassFlags ClassFlags { get; set; }
	public EClassCastFlags CastFlags { get; set; }
	public List<string> ImplementedInterfacePaths { get; set; } = new();
	public List<UnrealFunctionDefinition> Functions { get; set; } = new();
	public List<PropertyDefault> PropertyDefaults { get; set; } = new();
	public List<DefaultSubobject> DefaultSubobjects { get; set; } = new();
	public List<DefaultSubobjectOverride> DefaultSubobjectOverrides { get; set; } = new();
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
	public List<UnrealEnumDefinition> Enums { get; set; } = new();
	public List<UnrealScriptStructDefinition> Structs { get; set; } = new();
	public List<UnrealClassDefinition> Classes { get; set; } = new();
	public List<UnrealInterfaceDefinition> Interfaces { get; set; } = new();
	public List<UnrealDelegateDefinition> Delegates { get; set; } = new();

	[JsonIgnore]
	public bool IsEmpty => Enums.Count + Structs.Count + Classes.Count + Interfaces.Count + Delegates.Count == 0;
}


