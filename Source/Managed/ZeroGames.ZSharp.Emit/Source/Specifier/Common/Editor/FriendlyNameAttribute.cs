// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(SpecifierTargetConstants.AllFields)]
public class FriendlyNameAttribute(string value) : Attribute, IUnrealReflectionSpecifier
{
	public string Value { get; } = value;
}


