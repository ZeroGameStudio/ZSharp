// Copyright Zero Games. All Rights Reserved.

using System.Text.Json.Serialization;

namespace ZeroGames.ZSharp.Build.Glue;

public class ExportedVariable
{
	public required string Name { get; set; }
	public required FullyExportedTypeName Type { get; set; }
	public string? UnderlyingType { get; set; }
	[JsonPropertyName("bNullInNotNullOut")] public required bool IsNullInNotNullOut { get; set; }
	[JsonPropertyName("bBlackConjugate")] public required bool HasBlackConjugate { get; set;}
}


