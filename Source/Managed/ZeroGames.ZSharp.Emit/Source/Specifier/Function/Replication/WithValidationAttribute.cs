// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public class WithValidationAttribute : FunctionSpecifierBase
{
	public override IEnumerable<Type> HierarchicalRequirements => [ typeof(RemoteFunctionSpecifierBase) ];
}


