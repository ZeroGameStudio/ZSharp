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
	public Dictionary<string, string>? MetadataMap { get; set; }
	public Dictionary<string, string>? TransparentDataMap { get; set; }
}

// IMPORTANT: KEEP SYNC WITH ZUnrealFieldDefinitions.h.
public enum EPropertyType : uint8
{
	None,
	// Primitives
	UInt8, UInt16, UInt32, UInt64, Int8, Int16, Int32, Int64, Float, Double, Bool, Enum,
	// Strings
	String, Utf8String, AnsiString, Name, Text,
	// Object wrappers
	Object, Class, SoftClass, SoftObject, WeakObject, LazyObject, Interface,
	// Containers
	Struct, Array, Set, Map, Optional,
	// Delegates
	Delegate, MulticastInlineDelegate, MulticastSparseDelegate,
	// Special types
	FieldPath, // VerseValue is not supported.
}

public enum EEnumUnderlyingType : uint8
{
	None,
	UInt8,
	UInt16,
	UInt32,
	UInt64,
	Int8,
	Int16,
	Int32,
	Int64,
};

public enum EDelegateType : uint8
{
	None,
	Unicast,
	Multicast,
	Sparse,
}

public interface IUnrealSignatureDefinition;

public interface ISimpleUnrealPropertyDefinition
{
	EPropertyType Type { get; set; }
	public EEnumUnderlyingType EnumUnderlyingType { get; set; }
	EPropertyFlags PropertyFlags { get; set; }
	string? DescriptorFieldPath { get; set; }
}

public class SimpleUnrealPropertyDefinition : ISimpleUnrealPropertyDefinition
{
	public required EPropertyType Type { get; set; }
	public EEnumUnderlyingType EnumUnderlyingType { get; set; }
	public EPropertyFlags PropertyFlags { get; set; }
	public string? DescriptorFieldPath { get; set; }
}

public class UnrealPropertyDefinition : UnrealFieldDefinition, ISimpleUnrealPropertyDefinition
{
	public override string GetDisplayName()
	{
		return $"{Outer.GetDisplayName()}.{base.GetDisplayName()}";
	}

	[JsonIgnore]
	public UnrealStructDefinition Outer { get; init; } = null!; // Can't use required for JsonIgnore property...
	
	public required EPropertyType Type { get; set; }
	public EEnumUnderlyingType EnumUnderlyingType { get; set; }
	public EPropertyFlags PropertyFlags { get; set; }
	public string? DescriptorFieldPath { get; set; }
	
	public string? RepNotifyName { get; set; }
	public ERepCondition RepCondition { get; set; }
	public ERepNotifyCondition RepNotifyCondition { get; set; }
	public bool IsRepPushBased { get; set; }

	public SimpleUnrealPropertyDefinition? InnerProperty { get; set; }
	public SimpleUnrealPropertyDefinition? OuterProperty { get; set; }
}

public abstract class UnrealStructDefinition : UnrealFieldDefinition
{
	public struct PropertyDefault
	{
		public required string PropertyChain { get; set; }
		public required string Buffer { get; set; }
	}
	
	public string? SuperPath { get; set; }
	public List<UnrealPropertyDefinition> Properties { get; set; } = [];
	public List<PropertyDefault> PropertyDefaults { get; set; } = [];
}

public class UnrealFunctionDefinition : UnrealStructDefinition, IUnrealSignatureDefinition
{
	public override string GetDisplayName()
	{
		return $"{Outer.GetDisplayName()}.{base.GetDisplayName()}";
	}
	
	[JsonIgnore]
	public UnrealClassDefinition Outer { get; init; } = null!; // Can't use required for JsonIgnore property...
	
	[JsonPropertyName("bIsEventOverride")] public bool IsEventOverride { get; set; }
	public EFunctionFlags FunctionFlags { get; set; }
	public required string ZCallName { get; set; }
	public string? ValidateZCallName { get; set; }
	public uint16 RpcId { get; set; }
	public uint16 RpcResponseId { get; set; }
	public string? CustomThunkName { get; set; }
}

public class UnrealEnumFieldDefinition : UnrealFieldDefinition
{
	public int64 Value { get; set; }
}

public class UnrealEnumDefinition : UnrealFieldDefinition
{
	public EEnumUnderlyingType UnderlyingType { get; set; }
	public EEnumFlags EnumFlags { get; set; }
	public List<UnrealEnumFieldDefinition> Fields { get; set; } = [];
}

public class UnrealScriptStructDefinition : UnrealStructDefinition
{
	public EStructFlags StructFlags { get; set; }
}

public class UnrealDelegateDefinition : UnrealStructDefinition, IUnrealSignatureDefinition
{
	public EDelegateType DelegateType { get; set; }
	public string? OuterClassName { get; set; }
	public EFunctionFlags FunctionFlags { get; set; }
}

public class UnrealInterfaceDefinition : UnrealStructDefinition
{
	// @TODO
}

public class UnrealClassDefinition : UnrealStructDefinition
{
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
	
	[JsonPropertyName("bConstructor")] public bool HasUClassConstructor { get; set; }
	[JsonPropertyName("bContextual")] public bool IsContextual { get; set; }
	public string? ConfigName { get; set; }
	public string? WithinPath { get; set; }
	public EClassFlags ClassFlags { get; set; }
	public EClassCastFlags CastFlags { get; set; }
	public List<string> ImplementedInterfacePaths { get; set; } = [];
	public List<UnrealFunctionDefinition> Functions { get; set; } = [];
	public List<DefaultSubobject> DefaultSubobjects { get; set; } = [];
	public List<DefaultSubobjectOverride> DefaultSubobjectOverrides { get; set; } = [];
	public List<string> FieldNotifies { get; set; } = [];
}

public class UnrealFieldManifest
{
	public required string ModuleName { get; set; }
	public List<UnrealEnumDefinition> Enums { get; set; } = [];
	public List<UnrealScriptStructDefinition> Structs { get; set; } = [];
	public List<UnrealDelegateDefinition> Delegates { get; set; } = [];
	public List<UnrealInterfaceDefinition> Interfaces { get; set; } = [];
	public List<UnrealClassDefinition> Classes { get; set; } = [];

	[JsonIgnore]
	public bool IsEmpty => Enums.Count + Structs.Count + Delegates.Count + Interfaces.Count + Classes.Count == 0;
}


