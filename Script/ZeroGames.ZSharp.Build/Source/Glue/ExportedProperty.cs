// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Build.Glue;

[Flags]
public enum EExportedPropertyFlags : uint64
{
	None = 0,
	Public = 1 << 0,
	Protected = 1 << 1,
	Private = 1 << 2,
	Readable = 1 << 3,
	Writable = 1 << 4,
	ForceCopy = 1 << 5,
}

public class ExportedProperty : ExportedVariable
{
	public required string ZCallName { get; set; }
	public required EExportedPropertyFlags Flags { get; set; }
	public required int32 Index { get; set; }
	
	public bool IsPublic => Flags.HasFlag(EExportedPropertyFlags.Public);
	public bool IsProtected => Flags.HasFlag(EExportedPropertyFlags.Protected);
	public bool IsPrivate => Flags.HasFlag(EExportedPropertyFlags.Private);
	public bool IsReadable => Flags.HasFlag(EExportedPropertyFlags.Readable);
	public bool IsWritable => Flags.HasFlag(EExportedPropertyFlags.Writable);
	public bool IsForceCopy => Flags.HasFlag(EExportedPropertyFlags.ForceCopy);
}


