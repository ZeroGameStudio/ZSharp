// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public abstract class PropertyReplicatedSpecifierBase : PropertySpecifierBase
{
	public override IEnumerable<Type> HierarchicalConflicts => [ typeof(PropertyReplicatedSpecifierBase) ];
}

public class ReplicatedAttribute : PropertyReplicatedSpecifierBase
{
	public string? Notify { get; init; }
}

[AttributeUsage(AttributeTargets.Property | AttributeTargets.Parameter)]
public class NotReplicatedAttribute : PropertyReplicatedSpecifierBase;


