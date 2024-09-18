// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(AttributeTargets.Class, AllowMultiple = true, Inherited = false)]
public class DefaultSubobjectClassOverrideAttribute : Attribute
{
	public required string Subobject { get; init; }
	public required Type Class { get; init; }
}


