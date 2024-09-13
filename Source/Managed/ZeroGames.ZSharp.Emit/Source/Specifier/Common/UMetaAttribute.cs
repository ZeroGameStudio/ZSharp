// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(AttributeTargets.Class | AttributeTargets.Method | AttributeTargets.Property)]
public class UMetaAttribute(params string[] pairs) : Attribute
{
	public string[] Pairs => pairs;
}


