﻿// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public class AttachmentAttribute : PropertySpecifierBase
{
	public IEnumerable<Type> HierarchicalRequirements => [ typeof(DefaultSubobjectAttribute) ];
	public IEnumerable<Type> HierarchicalConflicts => [ typeof(RootComponentAttribute) ];
	
	public required string Parent { get; init; }
	public string? Socket { get; init; }
}

