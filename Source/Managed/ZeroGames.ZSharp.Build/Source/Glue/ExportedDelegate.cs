// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Build.Glue;

[Flags]
public enum EExportedDelegateFlags : uint64
{
	None = 0,
	Multicast = 1 << 0,
}

public class ExportedDelegate : ExportedType
{
	public required EExportedDelegateFlags Flags { get; set; }
	public required List<ExportedParameter> Parameters { get; set; }

	public ExportedParameter? ReturnParameter => Parameters.Count > 0 ? Parameters[^1] : null;
}


