// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public class ExecAttribute : FunctionSpecifierBase
{
	public override IEnumerable<Type> HierarchicalConflicts => [ typeof(RemoteFunctionSpecifierBase) ];
}


