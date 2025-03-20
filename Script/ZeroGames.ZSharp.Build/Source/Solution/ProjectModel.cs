// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Build.Solution;

public enum EIntrinsicProjectType
{
	None,
	Core,
	CoreAsync,
	CoreEngine,
	Engine,
	Emit,
}

public enum EProjectLanguageType
{
	CSharp,
}

public enum EProjectOutputType
{
	Runtime,
	Editor,
	Analyzer,
}

public readonly struct PackageReference
{
	public required string Name { get; init; }
	public required string Version { get; init; }
	public bool IsPrivate { get; init; }
}

public readonly struct ExternalReference
{
	public required string Name { get; init; }
	public required string Path { get; init; }
}

public class ProjectModel
{
	public required string Name { get; set; }
	public required string Path { get; set; }
	public required string SourceDir { get; set; }
	public required EIntrinsicProjectType IntrinsicType { get; set; }
	public required bool IsPrecompiled { get; set; }
	
	public required string Folder { get; set; }

	public required string TargetFramework { get; set; }
	public required EProjectOutputType OutputType { get; set; }
	public required EProjectLanguageType Language { get; set; }
	public required string LanguageVersion { get; set; }
	public required string RootNamespace { get; set; }
	public required bool AllowUnsafeBlocks { get; set; }
	public required List<string> WarningsAsErrors { get; set; }
	
	public required string Authors { get; set; }
	public required string Company { get; set; }
	public required string AssemblyVersion { get; set; }
	public required string FileVersion { get; set; }
	public required string NeutralLanguage { get; set; }

	public required bool UsesMinimalImplicitUsings { get; set; }

	public required string OutputDir { get; set; }
	
	public List<ProjectModel> ProjectReferences { get; set; } = null!; // This needs post fixup.
	public required List<PackageReference> PackageReferences { get; set; }
	public required List<ExternalReference> ExternalReferences { get; set; }
	
	public required List<string> PublicUsings { get; set; }
	public required List<string> PrivateUsings { get; set; }
	public IEnumerable<string> Usings => ProjectReferences.SelectMany(project => project.PublicUsings).Concat(PublicUsings).Concat(PrivateUsings);
	
	public required List<string> PublicStaticUsings { get; set; }
	public required List<string> PrivateStaticUsings { get; set; }
	public IEnumerable<string> StaticUsings => ProjectReferences.SelectMany(project => project.PublicStaticUsings).Concat(PublicStaticUsings).Concat(PrivateStaticUsings);
	
	public required Dictionary<string, string> PublicAliases { get; set; }
	public required Dictionary<string, string> PrivateAliases { get; set; }
	public IEnumerable<KeyValuePair<string, string>> Aliases => ProjectReferences.SelectMany(project => project.PublicAliases).Concat(PublicAliases).Concat(PrivateAliases);

	public required List<string> PublicConstants { get; set; }
	public required List<string> PrivateConstants { get; set; }
	public IEnumerable<string> Constants => ProjectReferences.SelectMany(project => project.PublicConstants).Concat(PublicConstants).Concat(PrivateConstants).Concat(UnrealModules.Select(module => $"UE_MODULE_{module.ToUpper()}"));
	
	public required List<string> PublicUnrealModules { get; set; }
	public IEnumerable<string> UnrealModules => ProjectReferences.SelectMany(project => project.UnrealModules).Concat(PublicUnrealModules);
}


