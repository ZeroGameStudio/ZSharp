// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(AttributeTargets.Assembly)]
public class EmitRootNamespaceAttribute(string @namespace) : Attribute
{
	public string Namespace { get; } = @namespace;
}


