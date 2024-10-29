// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public abstract class PropertyReplicatedSpecifierBase : PropertySpecifierBase
{
	public override IEnumerable<Type> HierarchicalConflicts => [ typeof(PropertyReplicatedSpecifierBase) ];
}

public enum ERepCondition
{
	None,
	InitialOnly,
	OwnerOnly,
	SkipOwner,
	SimulatedOnly,
	AutonomousOnly,
	SimulatedOrPhysics,
	InitialOrOwner,
	Custom,
	ReplayOrOwner,
	ReplayOnly,
	SimulatedOnlyNoReplay,
	SimulatedOrPhysicsNoReplay,
	SkipReplay,
}

public enum ERepNotifyCondition
{
	OnChanged,
	Always,
}

public class ReplicatedAttribute : PropertyReplicatedSpecifierBase
{
	public string? Notify { get; init; }
	public ERepCondition Condition { get; init; }
	public ERepNotifyCondition NotifyCondition { get; init; }
	public bool IsPushBased { get; init; } = true;
}

[AttributeUsage(AttributeTargets.Property | AttributeTargets.Parameter)]
public class NotReplicatedAttribute : PropertyReplicatedSpecifierBase;


