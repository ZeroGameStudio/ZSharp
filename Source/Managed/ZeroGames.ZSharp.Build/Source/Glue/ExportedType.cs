// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;

namespace ZeroGames.ZSharp.Build.Glue;

public abstract class ExportedType
{
	public string Name { get; set; } = string.Empty;
	public string Module { get; set; } = string.Empty;
	[NotNull]
	public ExportedAssembly? Assembly { get; set; }
	
	public string Namespace => $"{Assembly.Name}.{Module}";
}


