// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(AttributeTargets.Property)]
public class BlueprintReadOnlyAttribute : Attribute, IUnrealReflectionSpecifier
{
	public IEnumerable<Type> HierarchicalConflicts => [ typeof(BlueprintReadWriteAttribute) ];
}


