// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(AttributeTargets.Property)]
public class AttachmentAttribute : Attribute, IUnrealReflectionSpecifier
{
	public IEnumerable<Type> HierarchicalRequirements => [ typeof(DefaultSubobjectAttribute) ];
	public IEnumerable<Type> HierarchicalConflicts => [ typeof(RootComponentAttribute) ];
	
	public required string Parent { get; init; }
	public string? Socket { get; init; }
}


