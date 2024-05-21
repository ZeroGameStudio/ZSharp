// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

[AttributeUsage(AttributeTargets.Class)]
public class ConjugateRegistryIdAttribute : Attribute
{
	public ConjugateRegistryIdAttribute(uint16 id) => Id = id;
	public uint16 Id { get; }
}


