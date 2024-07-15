// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Build.Glue;

[Flags]
public enum EExportedDelegateFlags : uint64
{
	None = 0,
	Multicast = 1 << 0,
	Sparse = 1 << 1,
}

public class ExportedDelegate : ExportedType
{
	public required EExportedDelegateFlags Flags { get; set; }
	public required List<ExportedParameter> Parameters { get; set; }

	public bool IsMulticast => (Flags & EExportedDelegateFlags.Multicast) != EExportedDelegateFlags.None;
	public bool IsSparse => (Flags & EExportedDelegateFlags.Sparse) != EExportedDelegateFlags.None;
	
	public ExportedParameter? ReturnParameter => Parameters.Count > 0 && Parameters[^1].IsReturn ? Parameters[^1] : null;
}


