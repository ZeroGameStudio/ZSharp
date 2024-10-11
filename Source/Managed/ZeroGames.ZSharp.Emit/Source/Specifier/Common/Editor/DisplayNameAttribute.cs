// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(SpecifierTargetConstants.AllFields)]
public class DisplayNameAttribute(string value) : Attribute, IUnrealReflectionSpecifier
{
	public string Value { get; } = value;
}


