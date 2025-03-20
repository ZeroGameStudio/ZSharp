// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public readonly struct Block(string content) : ITypeMember
{
	public Block() : this(string.Empty){}
	public string Content { get; } = content;
	public override string ToString() => Content;
	public static implicit operator Block(string content) => new(content);
}


