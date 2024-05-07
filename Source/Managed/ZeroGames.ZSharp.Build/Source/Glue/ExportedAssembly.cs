// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Build.Glue;

public class ExportedAssembly
{
	public string Name { get; set; } = string.Empty;
	public List<ExportedEnum> Enums { get; set; } = new();
}


