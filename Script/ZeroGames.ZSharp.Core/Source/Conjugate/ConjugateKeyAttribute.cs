// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

[AttributeUsage(AttributeTargets.Class | AttributeTargets.Interface)]
public sealed class ConjugateKeyAttribute(string key) : Attribute
{
	public string Key { get; } = key;
}


