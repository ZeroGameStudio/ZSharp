// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Build.Glue;

[Flags]
public enum EExportedParameterFlags : uint64
{
	None = 0,
	In = 1 << 0,
	Out = 1 << 1,
	Return = 1 << 2,
}

public class ExportedParameter : ExportedVariable
{
	public required EExportedParameterFlags Flags { get; set; }
	
	public bool IsIn => (Flags & EExportedParameterFlags.In) != EExportedParameterFlags.None;
	public bool IsOut => (Flags & EExportedParameterFlags.Out) != EExportedParameterFlags.None;
	public bool IsInOut => IsIn && IsOut;
	public bool IsReturn => (Flags & EExportedParameterFlags.Return) != EExportedParameterFlags.None;
}


