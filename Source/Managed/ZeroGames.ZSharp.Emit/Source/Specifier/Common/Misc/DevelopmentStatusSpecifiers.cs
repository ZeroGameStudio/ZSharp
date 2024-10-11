// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(SpecifierTargetConstants.AllFields)]
public abstract class DevelopmentStatusSpecifierBase : Attribute, IUnrealReflectionSpecifier
{
	public IEnumerable<Type> HierarchicalConflicts => [ typeof(DevelopmentStatusSpecifierBase) ];
}

public class ExperimentalAttribute : DevelopmentStatusSpecifierBase;
public class EarlyAccessAttribute : DevelopmentStatusSpecifierBase;


