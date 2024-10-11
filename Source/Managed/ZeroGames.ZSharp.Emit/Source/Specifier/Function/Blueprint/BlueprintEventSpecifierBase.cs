// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(AttributeTargets.Method)]
public abstract class BlueprintEventSpecifierBase : Attribute, IUnrealReflectionSpecifier
{
	public virtual IEnumerable<Type> HierarchicalConflicts => [ typeof(BlueprintEventSpecifierBase) ];
}


