// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(AttributeTargets.Method)]
public abstract class BlueprintFunctionVisibilitySpecifierBase : Attribute, IUnrealReflectionSpecifier
{
	public IEnumerable<Type> HierarchicalConflicts => [ typeof(BlueprintFunctionVisibilitySpecifierBase) ];
}

public class BlueprintCallableAttribute : BlueprintFunctionVisibilitySpecifierBase;
public class BlueprintPureAttribute : BlueprintFunctionVisibilitySpecifierBase;


