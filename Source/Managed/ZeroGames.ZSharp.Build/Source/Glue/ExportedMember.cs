// Copyright Zero Games. All Rights Reserved.

using System.Text.Json.Serialization;

namespace ZeroGames.ZSharp.Build.Glue;

public class ExportedMember
{
	public string Name { get; set; } = string.Empty;
	[JsonPropertyName("bStatic")] public bool IsStatic { get; set; }
}


