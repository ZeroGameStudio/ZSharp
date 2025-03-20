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
	Hashable = 1 << 5,
	Implementable = 1 << 6,
}

public class ExportedClass : ExportedType
{
	public ExportedClass() => SlotType = EZCallBufferSlotType.Conjugate;
	public uint16 ConjugateRegistryId { get; set; }
	public required string ConjugateKey { get; set; }
	public required EExportedClassFlags Flags { get; set; }
	public FullyExportedTypeName BaseType { get; set; }
	public required List<FullyExportedTypeName> Interfaces { get; set; }
	public required List<ExportedMethod> Methods { get; set; }
	public required List<ExportedProperty> Properties { get; set; }
	
	public bool IsPlain => Flags.HasFlag(EExportedClassFlags.Plain);
	public bool IsClass => Flags.HasFlag(EExportedClassFlags.Class);
	public bool IsStruct => Flags.HasFlag(EExportedClassFlags.Struct);
	public bool IsInterface => Flags.HasFlag(EExportedClassFlags.Interface);
	public bool IsAbstract => Flags.HasFlag(EExportedClassFlags.Abstract);
	public bool IsHashableStruct => IsStruct && Flags.HasFlag(EExportedClassFlags.Hashable);
	public bool IsImplementableInterface => IsInterface && Flags.HasFlag(EExportedClassFlags.Implementable);
}


