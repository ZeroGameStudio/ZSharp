// Copyright Zero Games. All Rights Reserved.

using System.Text.Json.Serialization;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

public class UnrealFieldDefinition
{
	public required string Name { get; set; }
	public required EObjectFlags Flags { get; set; }
	public Dictionary<string, string> MetadataMap { get; set; } = new();
}

public class UnrealPropertyDefinition : UnrealFieldDefinition
{
	// @TODO
}

public class UnrealStructDefinition : UnrealFieldDefinition
{
	public string? SuperPath { get; set; }
	public Dictionary<string, UnrealPropertyDefinition> PropertyMap { get; set; } = new();
}

public class UnrealFunctionDefinition : UnrealStructDefinition
{
	public required EFunctionFlags FunctionFlags { get; set; }
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
	public required EClassFlags ClassFlags { get; set; }
	public required EClassCastFlags CastFlags { get; set; }
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

public class UnrealPackageDefinition
{
	public required string Path { get; set; }
	public Dictionary<string, UnrealEnumDefinition> EnumMap { get; set; } = new();
	public Dictionary<string, UnrealScriptStructDefinition> StructMap { get; set; } = new();
	public Dictionary<string, UnrealClassDefinition> ClassMap { get; set; } = new();
	public Dictionary<string, UnrealInterfaceDefinition> InterfaceMap { get; set; } = new();
	public Dictionary<string, UnrealDelegateDefinition> DelegateMap { get; set; } = new();

	[JsonIgnore]
	public bool IsEmpty => EnumMap.Count + StructMap.Count + ClassMap.Count + InterfaceMap.Count + DelegateMap.Count == 0;
}


