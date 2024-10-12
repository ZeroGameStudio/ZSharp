// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public abstract class BlueprintEventSpecifierBase : FunctionSpecifierBase
{
	public virtual IEnumerable<Type> HierarchicalConflicts => [ typeof(BlueprintEventSpecifierBase) ];
}


