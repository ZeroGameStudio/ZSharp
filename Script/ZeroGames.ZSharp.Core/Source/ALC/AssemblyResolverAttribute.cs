// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

[AttributeUsage(AttributeTargets.Assembly)]
public sealed class AssemblyResolverAttribute(Type type) : Attribute
{
	public Type ResolverType { get; } = type;
}


