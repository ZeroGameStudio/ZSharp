// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public class SealedEventAttribute : FunctionSpecifierBase
{
	public IEnumerable<Type> HierarchicalRequirements => [ typeof(BlueprintRemoteFunctionSpecifierBase) ];
}


