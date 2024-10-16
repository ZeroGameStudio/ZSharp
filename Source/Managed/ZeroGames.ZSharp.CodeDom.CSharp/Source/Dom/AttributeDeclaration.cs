// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public readonly struct AttributeDeclaration(string name, params string[] arguments)
{
	public string Name { get; } = name;
	public IReadOnlyList<string> Arguments { get; } = arguments;
}


