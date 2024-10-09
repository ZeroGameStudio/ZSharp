// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(AttributeTargets.Property)]
public class BlueprintReadWriteAttribute : Attribute, IUnrealReflectionSpecifier
{
	public IEnumerable<Type> HierarchicalConflicts => [ typeof(BlueprintReadOnlyAttribute) ];
}


