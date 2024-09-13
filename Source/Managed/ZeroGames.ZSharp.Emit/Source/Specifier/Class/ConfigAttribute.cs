// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(AttributeTargets.Class)]
public class ConfigAttribute(string configName) : Attribute
{
	public string ConfigName => configName;
}


