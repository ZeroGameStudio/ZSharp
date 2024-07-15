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
	Event = 1 << 5,
}

public class ExportedProperty : ExportedVariable
{
	public required string ZCallName { get; set; }
	public required EExportedPropertyFlags Flags { get; set; }
	public required int32 Index { get; set; }
	
	public bool IsPublic => (Flags & EExportedPropertyFlags.Public) != EExportedPropertyFlags.None;
	public bool IsProtected => (Flags & EExportedPropertyFlags.Protected) != EExportedPropertyFlags.None;
	public bool IsPrivate => (Flags & EExportedPropertyFlags.Private) != EExportedPropertyFlags.None;
	public bool IsReadable => (Flags & EExportedPropertyFlags.Readable) != EExportedPropertyFlags.None;
	public bool IsWritable => (Flags & EExportedPropertyFlags.Writable) != EExportedPropertyFlags.None;
	public bool IsEvent => (Flags & EExportedPropertyFlags.Event) != EExportedPropertyFlags.None;
}


