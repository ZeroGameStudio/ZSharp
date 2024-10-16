// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public readonly struct ParameterDeclaration(EParameterKind kind, TypeReference type, string name)
{
	public EParameterKind Kind { get; } = kind;
	public TypeReference Type { get; } = type;
	public string Name { get; } = name;
}


