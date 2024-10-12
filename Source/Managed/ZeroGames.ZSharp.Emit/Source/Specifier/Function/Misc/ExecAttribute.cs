// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public class ExecAttribute : FunctionSpecifierBase
{
	public IEnumerable<Type> HierarchicalConflicts => [ typeof(BlueprintRemoteFunctionSpecifierBase) ];
}


