// Copyright Zero Games. All Rights Reserved.

using System.Text.Json;

namespace ZeroGames.ZSharp.Build.Solution;

public class ProjectManifest
{

	public ProjectManifest(string unrealProjectDir, string zsharpPluginDir, IEnumerable<string> sources, Dictionary<string, string> moduleMap)
	{
		_unrealProjectDir = unrealProjectDir;
		_zsharpPluginDir = zsharpPluginDir;

		_assemblyModulesLookup = [];
		foreach (var pair in moduleMap)
		{
			if (_assemblyModulesLookup.TryGetValue(pair.Value, out var list))
			{
				list.Add(pair.Key);
			}
			else
			{
				_assemblyModulesLookup[pair.Value] = [ pair.Key ];
			}
		}
		
		List<string> projectFilePaths = new List<string>();
		foreach (var source in sources)
		{
			LootZSharpProjects(source, projectFilePaths);
		}
		
		_projectMap = [];
		_projectReferenceLookup = [];
		Parallel.ForEach(projectFilePaths, CreateProjectModel);
		FixupProjectModelReferences();

		Solution = CreateSolutionModel();
	}
	
	public SolutionModel Solution { get; }
	public IEnumerable<ProjectModel> Projects => _projectMap.Values;

	private static void LootZSharpProjects(string source, List<string> projectFilePaths)
	{
		void Loot(string subpath, bool searchFile)
		{
			if (!Directory.Exists(subpath))
			{
				return;
			}
            
			if (searchFile)
			{
				string fileName = $"{new DirectoryInfo(subpath).Name}.zsproj";
				string? filePath = Directory.GetFiles(subpath, fileName, SearchOption.AllDirectories).FirstOrDefault();
				if (filePath is not null)
				{
					projectFilePaths.Add(filePath);
					return;
				}
			}

			Array.ForEach(Directory.GetDirectories(subpath), p => Loot(p, true));
		}
		
		Loot(source, false);
	}

	private void CreateProjectModel(string zsharpProjectFilePath)
	{
		using FileStream fs = File.OpenRead(zsharpProjectFilePath);
		if (JsonSerializer.Deserialize<ProjectDto>(fs) is not {} dto)
		{
			throw new JsonException($"Z# project {zsharpProjectFilePath} fail to serialize.");
		}

		string name = Path.GetFileNameWithoutExtension(zsharpProjectFilePath);
		
		string projectFileExtension = dto.Language switch
		{
			EProjectLanguageType.CSharp => ".csproj",
			_ => throw new NotSupportedException($"Unknown language {dto.Language}")
		};
		
		EIntrinsicProjectType intrinsicType = name switch
		{
			"ZeroGames.ZSharp.Core" => EIntrinsicProjectType.Core,
			"ZeroGames.ZSharp.Core.UnrealEngine" => EIntrinsicProjectType.CoreEngine,
			"ZeroGames.ZSharp.Core.Async" => EIntrinsicProjectType.CoreAsync,
			"ZeroGames.ZSharp.UnrealEngine" => EIntrinsicProjectType.Engine,
			"ZeroGames.ZSharp.Emit" => EIntrinsicProjectType.Emit,
			_ => EIntrinsicProjectType.None
		};
		if (intrinsicType is not EIntrinsicProjectType.None && !zsharpProjectFilePath.Contains(_zsharpPluginDir))
		{
			throw new InvalidOperationException($"Illegal intrinsic project {zsharpProjectFilePath}.");
		}

		if (dto.OutputType is EProjectOutputType.Analyzer && dto.TargetFramework is not "netstandard2.0")
		{
			throw new InvalidOperationException($"Illegal target framework {dto.TargetFramework} for analyzer project {dto.OutputType}.");
		}
		
		ProjectModel project = new()
		{
			Name = name,
			Path = $"{_unrealProjectDir}/Intermediate/ZSharp/ProjectFiles/{name}/{name}{projectFileExtension}",
			SourceDir = Path.GetDirectoryName(zsharpProjectFilePath)!.Replace('\\', '/'),
			IntrinsicType = intrinsicType,
			
			Folder = dto.Folder,
			
			TargetFramework = dto.TargetFramework,
			OutputType = dto.OutputType,
			Language = dto.Language,
			LanguageVersion = dto.LanguageVersion,
			RootNamespace = string.IsNullOrWhiteSpace(dto.RootNamespace) ? name : dto.RootNamespace,
			AllowUnsafeBlocks = dto.AllowUnsafeBlocks,
			WarningsAsErrors = dto.WarningsAsErrors.ToList(),
			
			Authors = dto.Authors,
			Company = dto.Company,
			AssemblyVersion = dto.AssemblyVersion,
			FileVersion = dto.FileVersion,
			NeutralLanguage = dto.NeutralLanguage,
			
			UsesMinimalImplicitUsings = dto.UsesMinimalImplicitUsings,
			OutputPath = dto.OutputPath,
			
			PackageReferences = dto.PackageReferences.ToList(),
			ExternalReferences = dto.ExternalReferences.ToList(),
			
			PublicUsings = dto.PublicUsings.ToList(),
			PrivateUsings = dto.PrivateUsings.ToList(),
			
			PublicStaticUsings = dto.PublicStaticUsings.ToList(),
			PrivateStaticUsings = dto.PrivateStaticUsings.ToList(),
			
			PublicAliases = dto.PublicAliases.ToDictionary(),
			PrivateAliases = dto.PrivateAliases.ToDictionary(),
			
			PublicConstants = dto.PublicConstants.ToList(),
			PrivateConstants = dto.PrivateConstants.ToList(),
			
			PublicUnrealModules = _assemblyModulesLookup.TryGetValue(name, out var modules) ? modules : [],
		};

		lock (_projectMapLock)
		{
			_projectMap[project.Name] = project;
			if (dto.ProjectReferences.Count > 0)
			{
				_projectReferenceLookup[project] = dto.ProjectReferences.ToList();
			}
		}
	}

	private void FixupProjectModelReferences()
	{
		foreach (var pair in _projectMap)
		{
			ProjectModel project = pair.Value;
			project.ProjectReferences = [];
			
			if (_projectReferenceLookup.TryGetValue(project, out var references))
			{
				foreach (var reference in references)
				{
					if (!_projectMap.TryGetValue(reference, out var referencedProject))
					{
						throw new InvalidOperationException($"Unknown project reference {reference}.");
					}
					
					project.ProjectReferences.Add(referencedProject);
				}
			}
			
			string language = project.Language switch
			{
				EProjectLanguageType.CSharp => "CSharp",
				_ => throw new NotSupportedException($"Unknown language {project.Language}")
			};

			void ConditionallyAddProjectReference(string referencedProjectName)
			{
				ProjectModel? reference = project.ProjectReferences.SingleOrDefault(reference => reference.Name == referencedProjectName);
				if (reference is null)
				{
					project.ProjectReferences.Add(_projectMap[referencedProjectName]);
				}
			}

			// Auto import intrinsic analyzers for user project.
			if (project.IntrinsicType is EIntrinsicProjectType.None)
			{
				if (project.ProjectReferences.Any(reference => reference.IntrinsicType is not EIntrinsicProjectType.None))
				{
					ConditionallyAddProjectReference($"ZeroGames.ZSharp.Analyzer.{language}");
				}
			
				if (project.ProjectReferences.Any(reference => reference.IntrinsicType is EIntrinsicProjectType.Emit))
				{
					ConditionallyAddProjectReference($"ZeroGames.ZSharp.Emit.SourceGenerator.{language}");
				}
			}
		}
	}

	private SolutionModel CreateSolutionModel()
	{
		string unrealProjectFile = Directory.GetFiles(_unrealProjectDir, "*.uproject", SearchOption.TopDirectoryOnly).Single();
		string unrealProjectFileName = Path.GetFileNameWithoutExtension(unrealProjectFile);
		string solutionPath = $"{_unrealProjectDir}/{unrealProjectFileName}Script.slnx";
		
		SolutionModel solution = new()
		{
			Path = solutionPath,
		};

		SolutionFolder GetOrAddSolutionFolder(string path)
		{
			string[] pathNodes = path.Replace('\\', '/').Trim('/').Split('/');
			SolutionFolder cur = solution.RootFolder;
			foreach (var node in pathNodes)
			{
				cur = cur.GetOrAddChild(node);
			}
			
			return cur;
		}

		foreach (var pair in _projectMap)
		{
			ProjectModel project = pair.Value;
			SolutionFolder folder = GetOrAddSolutionFolder(project.Folder);
			folder.AddProject(project);
		}

		return solution;
	}
	
	private readonly string _unrealProjectDir;
	private readonly string _zsharpPluginDir;
	private readonly Dictionary<string, List<string>> _assemblyModulesLookup;
	private readonly Dictionary<string, ProjectModel> _projectMap;
	private readonly Dictionary<ProjectModel, List<string>> _projectReferenceLookup;
	private readonly Lock _projectMapLock = new();

}


