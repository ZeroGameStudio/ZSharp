// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public class AttributeList(params AttributeDeclaration[] declarations)
{

	public IReadOnlyList<AttributeDeclaration> Declarations { get; } = declarations;

}


