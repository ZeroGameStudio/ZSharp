// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

[AttributeUsage(AttributeTargets.Class)]
public sealed class ConjugateRegistryIdAttribute(uint16 id) : Attribute
{
	public uint16 Id { get; } = id;
}


