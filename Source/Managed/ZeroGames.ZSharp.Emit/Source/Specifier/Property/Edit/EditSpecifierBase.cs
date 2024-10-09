// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public abstract class EditSpecifierBase : Attribute, IUnrealReflectionSpecifier
{
	
	public IEnumerable<Type> HierarchicalConflicts => [ typeof(EditSpecifierBase) ];
	
}


