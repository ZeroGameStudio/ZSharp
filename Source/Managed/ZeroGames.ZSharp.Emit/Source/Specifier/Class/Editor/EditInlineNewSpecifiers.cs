// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public abstract class EditInlineNewSpecifierBase : ClassSpecifierBase
{
	public override IEnumerable<Type> HierarchicalConflicts => [ typeof(EditInlineNewSpecifierBase) ];
}

public class EditInlineNewAttribute : EditInlineNewSpecifierBase;
public class NotEditInlineNewAttribute : EditInlineNewSpecifierBase;


