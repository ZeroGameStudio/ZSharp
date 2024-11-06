// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public readonly struct Expression(string content)
{
	public string Content { get; } = content;
	public override string ToString() => Content;
	public static implicit operator Expression(string content) => new(content);
}


