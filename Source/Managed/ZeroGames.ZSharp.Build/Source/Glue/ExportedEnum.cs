// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Build.Glue;

[Flags]
public enum EExportedEnumFlags : uint64
{
	None = 0,
	Flags = 1 << 0,
}

public class ExportedEnum : ExportedType
{
	public EExportedEnumFlags Flags { get; set; }
	public required string UnderlyingType { get; set; }
	public required Dictionary<string, string> ValueMap { get; set; }
	
	public bool IsFlags => Flags.HasFlag(EExportedEnumFlags.Flags);
}


