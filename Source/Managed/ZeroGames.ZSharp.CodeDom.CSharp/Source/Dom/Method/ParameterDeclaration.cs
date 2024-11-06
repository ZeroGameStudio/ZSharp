// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public readonly struct ParameterDeclaration(EParameterKind kind, TypeReference type, string name, ParameterDefaultValue defaultValue, params AttributeDeclaration[]? attributes)
{
	public ParameterDeclaration(EParameterKind kind, TypeReference type, string name, params AttributeDeclaration[]? attributes) : this(kind, type, name, default, attributes){}
	
	public EParameterKind Kind { get; } = kind;
	public TypeReference Type { get; } = type;
	public string Name { get; } = name;
	public ParameterDefaultValue DefaultValue { get; } = defaultValue;
	public AttributeList? Attributes { get; } = attributes is not null && attributes.Length > 0 ? new(attributes) : null;
}


