// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public class RootComponentAttribute : PropertySpecifierBase
{
	public override IEnumerable<Type> HierarchicalRequirements => [ typeof(DefaultSubobjectAttribute) ];
}


