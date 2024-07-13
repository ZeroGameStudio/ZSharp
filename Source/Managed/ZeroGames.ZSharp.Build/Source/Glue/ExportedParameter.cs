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
}


