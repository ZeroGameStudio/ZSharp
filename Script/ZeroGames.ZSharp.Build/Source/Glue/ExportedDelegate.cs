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
	public required string ConjugateKey { get; set; }
	public required EExportedDelegateFlags Flags { get; set; }
	public required List<ExportedParameter> Parameters { get; set; }

	public bool IsMulticast => Flags.HasFlag(EExportedDelegateFlags.Multicast);
	public bool IsSparse => Flags.HasFlag(EExportedDelegateFlags.Sparse);
	
	public ExportedParameter? ReturnParameter => Parameters.Count > 0 && Parameters[^1].IsReturn ? Parameters[^1] : null;
}


