// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public readonly struct AttributeList(params AttributeDeclaration[] declarations)
{
	public IReadOnlyList<AttributeDeclaration> Declarations { get; } = declarations;
}


