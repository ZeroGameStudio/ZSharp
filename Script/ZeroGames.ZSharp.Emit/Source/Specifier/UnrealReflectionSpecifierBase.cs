// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public abstract class UnrealReflectionSpecifierBase : Attribute, IUnrealReflectionSpecifier
{
	public virtual IEnumerable<Type> HierarchicalConflicts => [];
	public virtual IEnumerable<Type> ExactConflicts => [];
	public virtual IEnumerable<Type> HierarchicalRequirements => [];
	public virtual IEnumerable<Type> ExactRequirements => [];
}


