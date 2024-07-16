// Copyright Zero Games. All Rights Reserved.

using System.Collections.ObjectModel;
using System.Text;
using System.Text.Json;

namespace ZeroGames.ZSharp.Build.Solution;

[BuildTarget("Solution")]
public class BuildTarget_GenerateSolution : BuildTargetBase, IUnrealProjectDir
{
    
    public override async Task<string> BuildAsync()
    {
        if (!Directory.Exists(UnrealProjectDir))
        {
            throw new FileNotFoundException($"Project dir {UnrealProjectDir} not exists.");
        }

        List<Task<string>> tasks = _projectMap.Select(pair => GenerateProjectFile(pair.Value)).ToList();
        tasks.Add(GenerateSolutionFile());
        await Task.WhenAll(tasks);

        return string.Join("\n", tasks.Select(task => task.Result));
    }
    
    public string UnrealProjectDir { get; }
    
    [FactoryConstructor]
    private BuildTarget_GenerateSolution(IBuildEngine engine, [Argument("projectdir")] string projectDir, [Argument("source")] string source) : base(engine)
    {
        UnrealProjectDir = projectDir;
        if (!((IUnrealProjectDir)this).IsValid)
        {
            throw new ArgumentException($"Invalid argument projectdir={projectDir}.");
        }
        _sourcePaths = source.Split(';').ToList();
        
        _projectMap = GatherProjectDefinitions();
        if (_projectMap.Count > 0)
        {
            string path = $"{UnrealProjectDir}/Intermediate";
            if (!Directory.Exists(path))
            {
                Directory.CreateDirectory(path);
            }
        
            path = $"{path}/ZSharp";
            if (!Directory.Exists(path))
            {
                Directory.CreateDirectory(path);
            }
        
            path = $"{path}/ProjectFiles";
            if (Directory.Exists(path))
            {
                Directory.Delete(path, true);
            }
            Directory.CreateDirectory(path);
        }
    }

    private Dictionary<string, ProjectDefinition> GatherProjectDefinitions() => Task.WhenAll(_sourcePaths.Select(GatherProjectDefinition)).Result.SelectMany(projects => projects).ToDictionary(project => project.Name);

    private async Task<List<ProjectDefinition>> GatherProjectDefinition(string path)
    {
        List<string> projectFilePaths = new List<string>();

        void GetProjectDefinition(string subpath, bool searchFile = true)
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

            Array.ForEach(Directory.GetDirectories(subpath), p => GetProjectDefinition(p));
        }

        GetProjectDefinition(path, false);

        List<FileStream> openFiles = new();
        List<ProjectDefinition> projects = new();
        List<ValueTask<ProjectDefinition?>> tasks = new();
        try
        {
            foreach (var filePath in projectFilePaths)
            {
                FileStream fs = File.OpenRead(filePath);
                openFiles.Add(fs);
                tasks.Add(JsonSerializer.DeserializeAsync<ProjectDefinition>(fs));
            }

            for (int32 i = 0; i < tasks.Count; ++i)
            {
                ProjectDefinition? project = await tasks[i];
                if (project is not null)
                {
                    project.Name = Path.GetFileNameWithoutExtension(projectFilePaths[i]);
                    project.SourceDir = Path.GetDirectoryName(projectFilePaths[i]) ?? string.Empty;
                    projects.Add(project);
                }
            }
        }
        finally
        {
            openFiles.ForEach(fs => fs.Close());
        }
        
        return projects;
    }

    private async Task<string> GenerateProjectFile(ProjectDefinition project)
    {
        string projectFileDir = $"{UnrealProjectDir}/{project.ProjectFileDir}";
        if (Directory.Exists(projectFileDir))
        {
            throw new InvalidOperationException($"Directory {projectFileDir} already exists.");
        }
        Directory.CreateDirectory(projectFileDir);

        string path = $"{UnrealProjectDir}/{project.ProjectFilePath}";

        await using FileStream fs = File.Create(path);
        await using StreamWriter sw = new(fs, Encoding.UTF8);

        // @FIXME: PluginDir
        ProjectFileBuilder builder = new(project, UnrealProjectDir, $"{UnrealProjectDir}/Plugins/ZeroGames/ZSharp");

        await sw.WriteAsync(builder.ToString());

        return $"Project file generated: {path}";
    }
    
    private async Task<string> GenerateSolutionFile()
    {
        // @FIXME: Solution file name
        string path = $"{UnrealProjectDir}/ZLabScript.sln";
        await using FileStream fs = File.Create(path);
        await using StreamWriter sw = new(fs, Encoding.UTF8);

        SolutionFileBuilder builder = new();
        Guid zsharpFolder = builder.AddFolder("ZSharp");
        Guid toolFolder = builder.AddFolder("Program", zsharpFolder);
        Guid gameFolder = builder.AddFolder("Game");
        
        foreach (var pair in _projectMap)
        {
            builder.AddProject(pair.Value);
        }

        await sw.WriteAsync(builder.ToString());

        return $"Solution file generated: {path}";
    }

    private List<string> _sourcePaths;
    private IReadOnlyDictionary<string, ProjectDefinition> _projectMap;
}


