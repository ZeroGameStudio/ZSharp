// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(AttributeTargets.Property)]
public class AttachmentAttribute : Attribute
{
	public required string Parent { get; init; }
	public string? Socket { get; init; }
}


