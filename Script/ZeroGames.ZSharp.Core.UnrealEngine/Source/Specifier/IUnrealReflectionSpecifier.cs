// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.UnrealEngine.Specifier;

public interface IUnrealReflectionSpecifier
{
	IEnumerable<Type> HierarchicalConflicts => [];
	IEnumerable<Type> ExactConflicts => [];
	IEnumerable<Type> HierarchicalRequirements => [];
	IEnumerable<Type> ExactRequirements => [];
}


