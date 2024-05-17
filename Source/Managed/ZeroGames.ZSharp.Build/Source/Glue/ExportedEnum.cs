// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Build.Glue;

public class ExportedEnum : ExportedType
{
	public ExportedEnum() => Static = true;
	public required string UnderlyingType { get; set; }
	public required Dictionary<string, string> ValueMap { get; set; }
}


