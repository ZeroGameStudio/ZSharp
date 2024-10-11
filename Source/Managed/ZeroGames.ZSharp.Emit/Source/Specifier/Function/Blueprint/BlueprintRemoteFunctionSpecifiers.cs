// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public abstract class BlueprintRemoteFunctionSpecifierBase : BlueprintEventSpecifierBase
{
	public IEnumerable<Type> HierarchicalRequirements => [ typeof(BlueprintRemoteFunctionReliabilitySpecifierBase) ];
	public override IEnumerable<Type> HierarchicalConflicts => [typeof(BlueprintEventSpecifierBase), typeof(ExecAttribute)];
	
	public string? ImplementationZCallName { get; init; }
}

public class ServerAttribute : BlueprintRemoteFunctionSpecifierBase;
public class ClientAttribute : BlueprintRemoteFunctionSpecifierBase;
public class NetMulticastAttribute : BlueprintRemoteFunctionSpecifierBase;

[AttributeUsage(AttributeTargets.Method)]
public class WithValidationAttribute : Attribute, IUnrealReflectionSpecifier
{
	public IEnumerable<Type> HierarchicalRequirements => [ typeof(BlueprintRemoteFunctionSpecifierBase) ];
	
	public string? ImplementationZCallName { get; init; }
}

[AttributeUsage(AttributeTargets.Method)]
public abstract class BlueprintRemoteFunctionReliabilitySpecifierBase : Attribute, IUnrealReflectionSpecifier
{
	public IEnumerable<Type> HierarchicalRequirements => [ typeof(BlueprintRemoteFunctionSpecifierBase) ];
}

public class ReliableAttribute : BlueprintRemoteFunctionReliabilitySpecifierBase;
public class UnreliableAttribute : BlueprintRemoteFunctionReliabilitySpecifierBase;


