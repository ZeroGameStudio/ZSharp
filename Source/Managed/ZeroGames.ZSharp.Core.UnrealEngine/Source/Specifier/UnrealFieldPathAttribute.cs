// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.UnrealEngine.Specifier;

[AttributeUsage(AttributeTargets.Class | AttributeTargets.Interface | AttributeTargets.Enum | AttributeTargets.Delegate, Inherited = false)]
public class UnrealFieldPathAttribute(string path) : Attribute, IUnrealReflectionSpecifier
{
	public string Path { get; } = path;
}


