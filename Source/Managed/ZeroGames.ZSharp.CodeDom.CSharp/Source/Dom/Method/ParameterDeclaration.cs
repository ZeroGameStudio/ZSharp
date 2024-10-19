// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public readonly struct ParameterDeclaration(EParameterKind kind, TypeReference type, string name, params AttributeDeclaration[]? attributes)
{
	public EParameterKind Kind { get; } = kind;
	public TypeReference Type { get; } = type;
	public string Name { get; } = name;
	public AttributeList? Attributes { get; } = attributes is not null && attributes.Length > 0 ? new(attributes) : null;
}


