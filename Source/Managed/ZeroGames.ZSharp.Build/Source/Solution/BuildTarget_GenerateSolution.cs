// Copyright Zero Games. All Rights Reserved.

using System.Collections;
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

        List<Task<string>> tasks = _projectMap.Select(pair => GenerateProjectFileAsync(pair.Value)).ToList();
        await Task.WhenAll(tasks);
        await GenerateSolutionFileAsync();

        return string.Join("\n", tasks.Select(task => task.Result));
    }
    
    public string UnrealProjectDir { get; }
    public string ZSharpPluginDir { get; }
    
    [FactoryConstructor]
    private BuildTarget_GenerateSolution(IBuildEngine engine, [Argument("projectdir")] string projectDir, [Argument("zsharpdir")] string zsharpDir, [Argument("source")] string source) : base(engine)
    {
        UnrealProjectDir = projectDir;
        ZSharpPluginDir = zsharpDir;
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

    private async Task<string> GenerateProjectFileAsync(ProjectDefinition project)
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

        ProjectFileBuilder builder = new(project, UnrealProjectDir, ZSharpPluginDir);

        await sw.WriteAsync(builder.ToString());

        return $"Project file generated: {path}";
    }
    
    private async Task<string> GenerateSolutionFileAsync()
    {
        string projectFile = Directory.GetFiles(UnrealProjectDir, "*.*", SearchOption.TopDirectoryOnly).FirstOrDefault(file => Path.GetExtension(file) == ".uproject") ?? throw new InvalidOperationException();
        string projectFileShortName = Path.GetFileNameWithoutExtension(projectFile);
        string path = $"{UnrealProjectDir}/{projectFileShortName}Script.sln";
        await using FileStream fs = File.Create(path);
        await using StreamWriter sw = new(fs, Encoding.UTF8);
        
        SolutionFileBuilder builder = new();

        SolutionDirectoryTree tree = new();
        tree.AddDirectory("ZSharp");
        tree.AddDirectory("Game");
        foreach (var pair in _projectMap)
        {
            tree.AddDirectory(pair.Value.Folder);
        }
        
        foreach (var node in tree)
        {
            if (!string.IsNullOrWhiteSpace(node.Parent.Path))
            {
                builder.AddFolder(node.Path, node.Parent.Path);
            }
            else
            {
                builder.AddFolder(node.Path);
            }
        }
        
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


