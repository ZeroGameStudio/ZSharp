// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Build.Glue;

public class ExportedEnum : ExportedType
{
	public ExportedEnum() => Static = true;
	public string UnderlyingType { get; set; } = string.Empty;
	public Dictionary<string, string> ValueMap { get; set; } = new();
}


