// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public abstract class DefaultSubobjectSpecifierBase : PropertySpecifierBase
{
	public IEnumerable<Type> HierarchicalConflicts => [ typeof(DefaultSubobjectSpecifierBase) ];
	
	public string? Name { get; init; }
}

public class DefaultSubobjectAttribute : DefaultSubobjectSpecifierBase;
public class OptionalDefaultSubobjectAttribute : DefaultSubobjectSpecifierBase;


