// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public class RootComponentAttribute : PropertySpecifierBase
{
	public IEnumerable<Type> HierarchicalRequirements => [ typeof(DefaultSubobjectAttribute) ];
}


