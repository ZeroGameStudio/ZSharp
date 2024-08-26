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
	public bool IsNullable { get; init; } = true;
	public bool IsStrongRestrictedNullable { get; init; } = true;
	public bool IsUnsafeBlockEnabled { get; init; } = true;
	public bool HasGlue { get; init; } = false;
	public bool IsPrecompiled { get; init; } = false;

	public string Authors { get; init; } = string.Empty;
	public string Company { get; init; } = string.Empty;
	public string AssemblyVersion { get; init; } = string.Empty;
	public string FileVersion { get; init; } = string.Empty;
	public string NeutralLanguage { get; init; } = string.Empty;

	public List<string> WarningsAsErrors { get; init; } = new();

	public List<string> ProjectReferences { get; init; } = new();
	public List<string> ExternalReferences { get; init; } = new();
	public List<string> Usings { get; init; } = new();
	public Dictionary<string, string> Aliases { get; init; } = new();

	public List<string> PostBuildCommands { get; init; } = new();

	public HashSet<string> Tags { get; init; } = new();

	public string ProjectFileExtension => Language.ToLower() switch
	{
		"c#" or "cs" or "csharp" => ".csproj",
		_ => throw new ArgumentException($"Unknown language: {Language}")
	};

	public string ProjectFileName => $"{Name}{ProjectFileExtension}";

	public string ProjectFileDir => $"Intermediate/ZSharp/ProjectFiles/{Name}";

	public string ProjectFilePath => $"{ProjectFileDir}/{ProjectFileName}";
}


