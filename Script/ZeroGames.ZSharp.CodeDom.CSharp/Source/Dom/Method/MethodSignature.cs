// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public readonly struct MethodSignature(TypeReference? returnType, params ParameterDeclaration[]? parameters)
{
	public TypeReference? ReturnType { get; } = returnType;
	public IReadOnlyList<ParameterDeclaration>? Parameters { get; } = parameters?.ToArray();
}


