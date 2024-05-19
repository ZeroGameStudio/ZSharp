// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Build.Glue;

[Flags]
public enum EExportedClassFlags : uint64
{
	None = 0,
	Class = 1 << 0,
	Struct = 1 << 1,
	Interface = 1 << 2,
	Abstract = 1 << 3,
}

public class ExportedClass : ExportedType
{
	public ExportedClass() => SlotType = EZCallBufferSlotType.Conjugate;
	public required EExportedClassFlags Flags { get; set; }
	public required string BaseType { get; set; }
	
	public bool Class => (Flags & EExportedClassFlags.Class) != EExportedClassFlags.None;
	public bool Struct => (Flags & EExportedClassFlags.Struct) != EExportedClassFlags.None;
	public bool Interface => (Flags & EExportedClassFlags.Interface) != EExportedClassFlags.None;
	public bool Abstract => (Flags & EExportedClassFlags.Abstract) != EExportedClassFlags.None;
}


