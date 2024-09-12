// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine;

[AttributeUsage(AttributeTargets.Class | AttributeTargets.Interface | AttributeTargets.Enum | AttributeTargets.Delegate, Inherited = false)]
public class UnrealFieldPathAttribute : Attribute
{
	public UnrealFieldPathAttribute(string path) => Path = path;
	public string Path { get; }
}


