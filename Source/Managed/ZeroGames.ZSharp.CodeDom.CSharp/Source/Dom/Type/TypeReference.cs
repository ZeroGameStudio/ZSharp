// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public readonly struct TypeReference(string typeName, string? underlyingType, bool notNullOut = false)
{
	public string TypeName { get; } = typeName;
	public string? UnderlyingType { get; } = underlyingType;
	public bool IsNotNullOut { get; } = notNullOut;
}


