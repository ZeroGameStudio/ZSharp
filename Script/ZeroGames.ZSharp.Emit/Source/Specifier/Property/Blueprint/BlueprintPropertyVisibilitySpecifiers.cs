// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public abstract class BlueprintPropertyVisibilitySpecifierBase : PropertySpecifierBase
{
	public override IEnumerable<Type> HierarchicalConflicts => [ typeof(BlueprintPropertyVisibilitySpecifierBase) ];
}


