// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Build.Glue;

public abstract class ExportedType
{
	public string Name { get; set; } = string.Empty;
	public string Module { get; set; } = string.Empty;
	public string Assembly { get; set; } = string.Empty;
	
	public string Namespace => $"{Assembly}.{Module}";
}


