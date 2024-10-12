// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public abstract class CollapseCategoriesSpecifierBase : ClassSpecifierBase
{
	public override IEnumerable<Type> HierarchicalConflicts => [ typeof(CollapseCategoriesSpecifierBase) ];
}

public class CollapseCategoriesAttribute : CollapseCategoriesSpecifierBase;
public class DontCollapseCategoriesAttribute : CollapseCategoriesSpecifierBase;


