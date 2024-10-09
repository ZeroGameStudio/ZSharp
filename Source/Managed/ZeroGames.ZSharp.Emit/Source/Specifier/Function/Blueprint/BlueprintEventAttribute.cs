// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(AttributeTargets.Method)]
public class BlueprintEventAttribute : Attribute, IUnrealReflectionSpecifier
{
	public IEnumerable<Type> HierarchicalConflicts => [ typeof(BlueprintEventOverrideAttribute) ];
	
	public string? ImplementationZCallName { get; init; }
}


