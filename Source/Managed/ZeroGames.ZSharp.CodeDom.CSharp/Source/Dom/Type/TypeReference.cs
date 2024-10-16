// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public readonly struct TypeReference(string typeName, string? underlyingType)
{
	public string TypeName { get; } = typeName;
	public string? UnderlyingType { get; } = underlyingType;
}


