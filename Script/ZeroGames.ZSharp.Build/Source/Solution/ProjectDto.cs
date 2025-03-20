// Copyright Zero Games. All Rights Reserved.

using System.Text.Json.Serialization;

namespace ZeroGames.ZSharp.Build.Solution;

public class ProjectDto
{
	public string Folder { get; set; } = "Game";

	public string TargetFramework { get; set; } = "net9.0";
	[JsonConverter(typeof(JsonStringEnumConverter<EProjectOutputType>))] public EProjectOutputType OutputType { get; set; } = EProjectOutputType.Runtime;
	[JsonConverter(typeof(JsonStringEnumConverter<EProjectLanguageType>))] public EProjectLanguageType Language { get; set; } = EProjectLanguageType.CSharp;
	public string LanguageVersion { get; set; } = "13";
	public string RootNamespace { get; set; } = string.Empty;
	public bool AllowUnsafeBlocks { get; set; } = true;
	public List<string> WarningsAsErrors { get; set; } = [];
	
	public string Authors { get; set; } = string.Empty;
	public string Company { get; set; } = string.Empty;
	public string AssemblyVersion { get; set; } = string.Empty;
	public string FileVersion { get; set; } = string.Empty;
	public string NeutralLanguage { get; set; } = string.Empty;

	public bool UsesMinimalImplicitUsings { get; set; } = true;

	public string OutputDir { get; set; } = "$(UnrealProjectDir)/Managed";

	public List<string> ProjectReferences { get; set; } = [];
	public List<PackageReference> PackageReferences { get; set; } = [];
	public List<ExternalReference> ExternalReferences { get; set; } = [];
	
	public List<string> PublicUsings { get; set; } = [];
	public List<string> PrivateUsings { get; set; } = [];
	
	public List<string> PublicStaticUsings { get; set; } = [];
	public List<string> PrivateStaticUsings { get; set; } = [];
	
	public Dictionary<string, string> PublicAliases { get; set; } = [];
	public Dictionary<string, string> PrivateAliases { get; set; } = [];

	public List<string> PublicConstants { get; set; } = [];
	public List<string> PrivateConstants { get; set; } = [];
}


