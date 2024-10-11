// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(AttributeTargets.Property)]
public abstract class DefaultSubobjectSpecifierBase : Attribute, IUnrealReflectionSpecifier
{
	
	public IEnumerable<Type> HierarchicalConflicts => [ typeof(DefaultSubobjectSpecifierBase) ];
	
	public string? Name { get; init; }
	
}

public class DefaultSubobjectAttribute : DefaultSubobjectSpecifierBase;
public class OptionalDefaultSubobjectAttribute : DefaultSubobjectSpecifierBase;


