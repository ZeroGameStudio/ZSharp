﻿// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public readonly struct Block(string content) : ITypeMember
{
	public string Content { get; } = content;
}


