// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public abstract class RemoteFunctionSpecifierBase : EventSpecifierBase
{
	public override IEnumerable<Type> HierarchicalRequirements => [ typeof(RemoteFunctionReliabilitySpecifierBase) ];
	public override IEnumerable<Type> HierarchicalConflicts => [typeof(EventSpecifierBase), typeof(ExecAttribute)];
}

public class ServerAttribute : RemoteFunctionSpecifierBase;
public class ClientAttribute : RemoteFunctionSpecifierBase;
public class NetMulticastAttribute : RemoteFunctionSpecifierBase;


