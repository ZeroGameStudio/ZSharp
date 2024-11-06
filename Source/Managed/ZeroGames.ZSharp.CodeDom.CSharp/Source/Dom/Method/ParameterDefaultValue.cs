// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public readonly struct ParameterDefaultValue(Expression signature, Expression body)
{
	public Expression Signature { get; } = signature;
	public Expression Body { get; } = body;
}


