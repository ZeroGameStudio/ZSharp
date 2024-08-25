// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

[AttributeUsage(AttributeTargets.Assembly)]
public class AssemblyResolverAttribute(Type type) : Attribute
{
	public Type ResolverType => type;
}


