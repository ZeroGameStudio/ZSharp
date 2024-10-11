// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(AttributeTargets.Method)]
public abstract class BlueprintFunctionAuthenticationSpecifierBase : Attribute, IUnrealReflectionSpecifier
{
	public IEnumerable<Type> HierarchicalConflicts => [ typeof(BlueprintFunctionAuthenticationSpecifierBase) ];
}

public class BlueprintAuthorityOnlyAttribute : BlueprintFunctionAuthenticationSpecifierBase;
public class BlueprintCosmeticAttribute : BlueprintFunctionAuthenticationSpecifierBase;


