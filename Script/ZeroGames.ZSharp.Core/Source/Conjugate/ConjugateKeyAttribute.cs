// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

[AttributeUsage(AttributeTargets.Class)]
public sealed class ConjugateKeyAttribute(string key) : Attribute
{
	public string Key { get; } = key;
}


