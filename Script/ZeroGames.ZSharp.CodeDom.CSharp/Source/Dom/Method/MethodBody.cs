// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public readonly struct MethodBody(params Block[]? contents)
{
	public Block[] Contents { get; } = contents ?? [];
}


