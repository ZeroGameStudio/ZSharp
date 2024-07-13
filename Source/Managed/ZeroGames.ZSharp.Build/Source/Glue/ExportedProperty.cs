// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Build.Glue;

[Flags]
public enum EExportedPropertyFlags : uint64
{
	None = 0,
	Readable = 1 << 0,
	Writable = 1 << 1,
	Public = 1 << 2,
	Protected = 1 << 3,
	Private = 1 << 4,
}

public class ExportedProperty
{
	public required string Name { get; set; }
	public required string ZCallName { get; set; }
	public required FullyExportedTypeName Type { get; set; }
	public required EExportedPropertyFlags Flags { get; set; }
	public required int32 Index { get; set; }
	
	public bool IsReadable => (Flags & EExportedPropertyFlags.Readable) != EExportedPropertyFlags.None;
	public bool IsWritable => (Flags & EExportedPropertyFlags.Writable) != EExportedPropertyFlags.None;
	public bool IsPublic => (Flags & EExportedPropertyFlags.Public) != EExportedPropertyFlags.None;
	public bool IsProtected => (Flags & EExportedPropertyFlags.Protected) != EExportedPropertyFlags.None;
	public bool IsPrivate => (Flags & EExportedPropertyFlags.Private) != EExportedPropertyFlags.None;
}


