// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(AttributeTargets.Class)]
public abstract class ClassTransientSpecifierBase : Attribute, IUnrealReflectionSpecifier
{
	public IEnumerable<Type> HierarchicalConflicts => [ typeof(ClassTransientSpecifierBase) ];
}

public class DefaultToTransientAttribute : ClassTransientSpecifierBase;
public class DefaultToNonTransientAttribute : ClassTransientSpecifierBase;


