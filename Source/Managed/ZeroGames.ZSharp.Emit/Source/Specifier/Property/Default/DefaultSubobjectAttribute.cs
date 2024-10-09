// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(AttributeTargets.Property)]
public class DefaultSubobjectAttribute : Attribute, IUnrealReflectionSpecifier
{
	public IEnumerable<Type> HierarchicalConflicts => [ typeof(DefaultSubobjectAttribute) ];
	
	public string? Name { get; init; }
}


