// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Build.Glue;

[Flags]
public enum EExportedClassFlags : uint64
{
	None = 0,
	Plain = 1 << 0,
	Class = 1 << 1,
	Struct = 1 << 2,
	Interface = 1 << 3,
	Abstract = 1 << 4,
}

public class ExportedClass : ExportedType
{
	public ExportedClass() => SlotType = EZCallBufferSlotType.Conjugate;
	public uint16 ConjugateRegistryId { get; set; }
	public required EExportedClassFlags Flags { get; set; }
	public string? BaseType { get; set; }
	public required Dictionary<string, ExportedProperty> PropertyMap { get; set; }
	
	public bool Plain => (Flags & EExportedClassFlags.Plain) != EExportedClassFlags.None;
	public bool Class => (Flags & EExportedClassFlags.Class) != EExportedClassFlags.None;
	public bool Struct => (Flags & EExportedClassFlags.Struct) != EExportedClassFlags.None;
	public bool Interface => (Flags & EExportedClassFlags.Interface) != EExportedClassFlags.None;
	public bool Abstract => (Flags & EExportedClassFlags.Abstract) != EExportedClassFlags.None;
}


