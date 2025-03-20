// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public abstract class AttachmentSpecifierBase : PropertySpecifierBase
{
	public override IEnumerable<Type> HierarchicalRequirements => [ typeof(DefaultSubobjectAttribute) ];
	public override IEnumerable<Type> HierarchicalConflicts => [ typeof(AttachmentSpecifierBase) ];
}

public class RootComponentAttribute : AttachmentSpecifierBase;

public class AttachmentAttribute : AttachmentSpecifierBase
{
	public string? Parent { get; init; }
	public string? Socket { get; init; }
}


