// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public abstract class PlaceableSpecifierBase : ClassSpecifierBase
{
	public override IEnumerable<Type> HierarchicalConflicts => [ typeof(PlaceableSpecifierBase) ];
}

public class PlaceableAttribute : PlaceableSpecifierBase;
public class NotPlaceableAttribute : PlaceableSpecifierBase;


