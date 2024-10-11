// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(AttributeTargets.Property)]
public abstract class BlueprintPropertyVisibilitySpecifierBase : Attribute, IUnrealReflectionSpecifier
{
	public IEnumerable<Type> HierarchicalConflicts => [ typeof(BlueprintPropertyVisibilitySpecifierBase) ];
}

public class BlueprintReadWriteAttribute : BlueprintPropertyVisibilitySpecifierBase;
public class BlueprintReadOnlyAttribute : BlueprintPropertyVisibilitySpecifierBase;


