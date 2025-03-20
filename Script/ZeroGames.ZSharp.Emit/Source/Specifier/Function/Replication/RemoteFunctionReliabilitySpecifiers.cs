// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public abstract class RemoteFunctionReliabilitySpecifierBase : FunctionSpecifierBase
{
	public override IEnumerable<Type> HierarchicalRequirements => [ typeof(RemoteFunctionSpecifierBase) ];
}

public class ReliableAttribute : RemoteFunctionReliabilitySpecifierBase;
public class UnreliableAttribute : RemoteFunctionReliabilitySpecifierBase;


