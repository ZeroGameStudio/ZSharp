// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public abstract class ClassTransientSpecifierBase : ClassSpecifierBase
{
	public override IEnumerable<Type> HierarchicalConflicts => [ typeof(ClassTransientSpecifierBase) ];
}

public class DefaultToTransientAttribute : ClassTransientSpecifierBase;
public class DefaultToNonTransientAttribute : ClassTransientSpecifierBase;


