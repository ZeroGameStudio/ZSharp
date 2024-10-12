// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public class UMetaAttribute(params string[] pairs) : CommonSpecifierBase
{
	public string[] Pairs => pairs;
}


