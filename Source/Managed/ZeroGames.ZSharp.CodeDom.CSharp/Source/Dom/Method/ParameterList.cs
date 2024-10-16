// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public readonly struct ParameterList(params ParameterDeclaration[] declarations)
{
	public IReadOnlyList<ParameterDeclaration> Declarations { get; } = declarations;
}


