// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

[AttributeUsage(AttributeTargets.Method)]
public class SpecifierProcessorAttribute : Attribute
{
	public Type[] HierarchicalConflicts { get; init; } = [];
	public Type[] ExactConflicts { get; init; } = [];
	public Type[] HierarchicalRequirements { get; init; } = [];
	public Type[] ExactRequirements { get; init; } = [];
}


