﻿// Copyright Zero Games. All Rights Reserved.

using System.Text.Json.Serialization;

namespace ZeroGames.ZSharp.Build.Solution;

public enum EProjectType
{
	CSharp,
}

public class ProjectDefinition
{
	[JsonIgnore] public string Name { get; set; } = string.Empty;
	[JsonIgnore] public string SourceDir { get; set; } = string.Empty;

	public string Folder { get; init; } = "Game";

	public string? TargetFramework { get; init; }
	public string? Language { get; init; }
	public string? LanguageVersion { get; init; }
	public string? RootNamespace { get; init; }
	public bool IsNullable { get; init; } = true;
	public bool IsStrongRestrictedNullable { get; init; } = true;
	public bool IsUnsafeBlockEnabled { get; init; } = true;
	public bool HasGlue { get; init; } = false;
	public bool IsPrecompiled { get; init; } = false;
	public bool IsCore { get; init; } = false;
	public bool IsForwardShared { get; init; } = false;
	public bool IsDeferredShared { get; init; } = false;
	public bool IsRoslynComponent { get; init; } = false;
	public bool HasAnalyzerReleaseTracking { get; init; } = false;
	public bool UsesZSharpAnalyzer { get; init; } = true;
	public bool ForceNoEmitSourceGenerator { get; init; } = false;

	public string Authors { get; init; } = string.Empty;
	public string Company { get; init; } = string.Empty;
	public string AssemblyVersion { get; init; } = string.Empty;
	public string FileVersion { get; init; } = string.Empty;
	public string NeutralLanguage { get; init; } = string.Empty;

	public List<string> WarningsAsErrors { get; init; } = new();

	public List<string> ProjectReferences { get; init; } = new();
	public List<string> ExternalReferences { get; init; } = new();
	public List<string> ProjectAnalyzerReferences { get; init; } = new();
	public List<string> ExternalAnalyzerReferences { get; init; } = new();
	public List<string> Usings { get; init; } = new();
	public List<string> StaticUsings { get; init; } = new();
	public Dictionary<string, string> Aliases { get; init; } = new();

	public List<string> Constants { get; init; } = new();
	public List<string> DebugGameConstants { get; init; } = new();
	public List<string> DevelopmentConstants { get; init; } = new();
	public List<string> ShippingConstants { get; init; } = new();
	
	public Dictionary<string, string> PropertyMap { get; init; } = new();
	public Dictionary<string, string> DebugGamePropertyMap { get; init; } = new();
	public Dictionary<string, string> DevelopmentPropertyMap { get; init; } = new();
	public Dictionary<string, string> ShippingPropertyMap { get; init; } = new();

	public List<string> PostBuildCommands { get; init; } = new();

	public List<string> Tags { get; init; } = new();

	public EProjectType ProjectType => Language?.ToLower() switch
	{
		null or "c#" or "cs" or "csharp" => EProjectType.CSharp,
		_ => throw new ArgumentException($"Unknown language: {Language}")
	};

	public string ProjectFileExtension => ProjectType switch
	{
		EProjectType.CSharp => ".csproj",
		_ => throw new ArgumentException($"Unknown project type: {ProjectType}")
	};

	public string ProjectFileName => $"{Name}{ProjectFileExtension}";

	public string ProjectFileDir => $"Intermediate/ZSharp/ProjectFiles/{Name}";

	public string ProjectFilePath => $"{ProjectFileDir}/{ProjectFileName}";
}


