// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public readonly struct NamespaceStatement(string namespaceName)
{
	public string NamespaceName { get; } = namespaceName;
}


