// Copyright Zero Games. All Rights Reserved.

using System.Text.Json.Serialization;

namespace ZeroGames.ZSharp.Build.Solution;

public class ProjectDefinition
{
	[JsonIgnore] public string Name { get; set; } = string.Empty;
	[JsonIgnore] public string SourceDir { get; set; } = string.Empty;

	public string ParentFolder { get; init; } = "Game";

	public string TargetFramework { get; init; } = "net8.0";
	public string Language { get; init; } = "C#";
	[JsonPropertyName("ImplicitUsings")] public bool bImplicitUsings { get; init; } = true;
	[JsonPropertyName("Nullable")] public bool bNullable { get; init; } = true;

	[JsonPropertyName("StrongRestrictedNullable")]
	public bool bStrongRestrictedNullable { get; init; } = true;

	[JsonPropertyName("AllowUnsafeBlocks")]
	public bool bAllowUnsafeBlocks { get; init; } = true;

	[JsonPropertyName("UnrealStylePrimitiveTypeAliases")]
	public bool bUnrealStylePrimitiveTypeAliases { get; init; } = true;

	public string Authors { get; init; } = string.Empty;
	public string Company { get; init; } = string.Empty;
	public string AssemblyVersion { get; init; } = string.Empty;
	public string FileVersion { get; init; } = string.Empty;
	public string NeutralLanguage { get; init; } = string.Empty;

	public List<string> WarningsAsErrors { get; init; } = new();

	public List<string> ProjectReferences { get; init; } = new();
	public List<string> References { get; init; } = new();
	public List<string> Usings { get; init; } = new();
	public Dictionary<string, string> Aliases { get; init; } = new();

	public List<string> PostBuildCommands { get; init; } = new();

	public List<string> Tags { get; init; } = new();

	public string ProjectFileExtension => Language.ToLower() switch
	{
		"c#" or "cs" or "csharp" => ".csproj",
		_ => throw new ArgumentException($"Unknown language: {Language}")
	};

	public string ProjectFileName => $"{Name}{ProjectFileExtension}";

	public string ProjectFileDir => Path.Combine("Intermediate/ZSharp/ProjectFiles", Name);

	public string ProjectFilePath => Path.Combine(ProjectFileDir, ProjectFileName);
}


