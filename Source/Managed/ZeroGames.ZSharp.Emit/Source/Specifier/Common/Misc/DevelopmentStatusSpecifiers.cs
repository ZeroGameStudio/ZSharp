// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public abstract class DevelopmentStatusSpecifierBase : CommonSpecifierBase
{
	public IEnumerable<Type> HierarchicalConflicts => [ typeof(DevelopmentStatusSpecifierBase) ];
}

public class ExperimentalAttribute : DevelopmentStatusSpecifierBase;
public class EarlyAccessAttribute : DevelopmentStatusSpecifierBase;


