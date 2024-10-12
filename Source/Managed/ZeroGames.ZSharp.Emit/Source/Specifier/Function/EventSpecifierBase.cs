// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public class EventSpecifierBase : FunctionSpecifierBase
{
	public override IEnumerable<Type> HierarchicalConflicts => [ typeof(EventSpecifierBase) ];
}


