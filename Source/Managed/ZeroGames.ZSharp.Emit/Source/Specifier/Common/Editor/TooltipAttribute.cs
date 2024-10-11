// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(SpecifierTargetConstants.AllFields)]
public class TooltipAttribute(string value) : Attribute, IUnrealReflectionSpecifier
{
	public string Value { get; } = value;
}


