// Copyright Zero Games. All Rights Reserved.

using System.Collections.ObjectModel;
using System.Text;
using System.Text.Json;
using System.Text.Json.Serialization;
using System.Xml;

namespace ZeroGames.ZSharp.Build;

public class BuildTarget_GenerateSolution : BuildTargetBase
{

    public BuildTarget_GenerateSolution(IBuildEngine engine) : base(engine)
    {
        _projectDir = engine.GetArgument("projectdir") ?? throw new InvalidOperationException("Argument [projectdir] does not exist.");
        string sourcePaths = engine.GetArgument("source") ?? throw new InvalidOperationException("Argument [source] does not exist.");
        _sourcePaths = sourcePaths.Split(';').ToList();
        
        _projectMap = new(GatherProjectDefinitions());
        if (_projectMap.Count > 0)
        {
            string path = Path.Combine(_projectDir, "Intermediate");
            if (!Directory.Exists(path))
            {
                Directory.CreateDirectory(path);
            }
        
            path = Path.Combine(path, "ZSharp");
            if (!Directory.Exists(path))
            {
                Directory.CreateDirectory(path);
            }
        
            path = Path.Combine(path, "ProjectFiles");
            if (Directory.Exists(path))
            {
                Directory.Delete(path, true);
            }
            Directory.CreateDirectory(path);
        }
    }
    
    public override async Task<string> BuildAsync()
    {
        if (!Directory.Exists(_projectDir))
        {
            throw new FileNotFoundException($"Project dir {_projectDir} not exists.");
        }

        List<Task<string>> tasks = _projectMap.Select(pair => GenerateProjectFile(pair.Value)).ToList();
        tasks.Add(GenerateSolutionFile());
        await Task.WhenAll(tasks);

        return string.Join("\n", tasks.Select(task => task.Result));
    }

    private Dictionary<string, ProjectDefinition> GatherProjectDefinitions() => Task.WhenAll(_sourcePaths.Select(GatherProjectDefinition)).Result.SelectMany(projects => projects).ToDictionary(project => project.Name);

    private async Task<List<ProjectDefinition>> GatherProjectDefinition(string path)
    {
        List<string> projectFilePaths = new List<string>();

        void GetProjectDefinition(string subpath, bool bSearchFile = true)
        {
            if (bSearchFile)
            {
                string fileName = $"{new DirectoryInfo(subpath).Name}.json";
                string? filePath = Directory.GetFiles(subpath, fileName, SearchOption.AllDirectories).FirstOrDefault();
                if (filePath is not null)
                {
                    projectFilePaths.Add(filePath);
                    return;
                }
            }

            Array.ForEach(Directory.GetDirectories(subpath), p => GetProjectDefinition(p));
        }

        GetProjectDefinition(Path.Combine(_projectDir, path), false);

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
        string projectFileDir = Path.Combine(_projectDir, project.ProjectFileDir);
        if (Directory.Exists(projectFileDir))
        {
            throw new InvalidOperationException($"Directory {projectFileDir} already exists.");
        }
        Directory.CreateDirectory(projectFileDir);

        string path = Path.Combine(_projectDir, project.ProjectFilePath);

        await using FileStream fs = File.Create(path);
        await using StreamWriter sw = new(fs, Encoding.UTF8);

        // @FIXME: PluginDir
        ProjectFileBuilder builder = new(project, _projectDir, Path.Combine(_projectDir, "Plugins/ZeroGames/ZSharp"));

        await sw.WriteAsync(builder.ToString());

        return $"Project file generated: {path}";
    }
    
    private async Task<string> GenerateSolutionFile()
    {
        // @FIXME: Solution file name
        string path = Path.Combine(_projectDir, "ZLabScript.sln");
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

    private string _projectDir;
    private List<string> _sourcePaths;
    private ReadOnlyDictionary<string, ProjectDefinition> _projectMap;

    private class ProjectDefinition
    {
        [JsonIgnore] public string Name { get; set; } = string.Empty;
        [JsonIgnore] public string SourceDir { get; set; } = string.Empty;
        
        public string ParentFolder { get; init; } = "Game";

        public string TargetFramework { get; init; } = "net8.0";
        public string Language { get; init; } = "C#";
        [JsonPropertyName("ImplicitUsings")]
        public bool bImplicitUsings { get; init; } = true;
        [JsonPropertyName("Nullable")]
        public bool bNullable { get; init; } = true;
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
            _ => throw new InvalidOperationException($"Unknown language: {Language}")
        };

        public string ProjectFileName => $"{Name}{ProjectFileExtension}";

        public string ProjectFileDir => Path.Combine("Intermediate/ZSharp/ProjectFiles", Name);
        
        public string ProjectFilePath => Path.Combine(ProjectFileDir, ProjectFileName);

    }

    private class ProjectFileBuilder
    {

        public ProjectFileBuilder(ProjectDefinition project, string unrealProjectDir, string zsharpPluginDir)
        {
            _project = project;
            _unrealProjectDir = unrealProjectDir;
            _zsharpPluginDir = zsharpPluginDir;
        }
        
        public override string ToString()
        {
            XmlDocument doc = new();
            
            BuildHeader(doc);
            XmlElement projectNode = BuildProjectNode(doc);
            BuildPropertyGroupNode(doc, projectNode);
            BuildPropertyGroupByConfiguration("DebugGame", doc, projectNode);
            BuildPropertyGroupByConfiguration("DebugGame Editor", doc, projectNode);
            BuildPropertyGroupByConfiguration("Development", doc, projectNode);
            BuildPropertyGroupByConfiguration("Development Editor", doc, projectNode);
            BuildPropertyGroupByConfiguration("Shipping", doc, projectNode);
            BuildPropertyGroupByConfiguration("Shipping Editor", doc, projectNode);
            BuildItemGroupNode(doc, projectNode);
            BuildReferenceNode(doc, projectNode);
            BuildTargetNode(doc, projectNode);
            
            XmlWriterSettings xws = new()
            {
                Encoding = Encoding.UTF8,
                Indent = true,
                IndentChars = "\t",
                OmitXmlDeclaration = true,
                CloseOutput = false,
            };
            using MemoryStream ms = new MemoryStream();
            using XmlWriter xw = XmlWriter.Create(ms, xws);
            using StreamReader sr = new(ms, Encoding.UTF8);
            doc.Save(xw);
            ms.Position = 0;
            
            return sr.ReadToEnd();
        }

        private static void AppendSimpleChild(XmlDocument doc, XmlElement elem, string childName, string childInnerText)
        {
            XmlElement childNode = doc.CreateElement(childName);
            childNode.InnerText = childInnerText;
            elem.AppendChild(childNode);
        }

        private void BuildHeader(XmlDocument doc)
        {
            doc.AppendChild(doc.CreateComment("Copyright Zero Games. All Rights Reserved."));

            doc.AppendChild(doc.CreateComment("THIS FILE IS GENERATED BY ZSHARP."));
            doc.AppendChild(doc.CreateComment("DO NOT MODIFY DIRECTLY!"));
        }

        private XmlElement BuildProjectNode(XmlDocument doc)
        {
            XmlElement projectNode = doc.CreateElement("Project");
            projectNode.SetAttribute("Sdk", "Microsoft.NET.Sdk");
            doc.AppendChild(projectNode);

            return projectNode;
        }

        private XmlElement BuildPropertyGroupNode(XmlDocument doc, XmlElement projectNode)
        {
            XmlElement propertyGroupNode = doc.CreateElement("PropertyGroup");
            void Append(string childName, string childInnerText) => AppendSimpleChild(doc, propertyGroupNode, childName, childInnerText);
            Append("ProjectName", _project.Name);
            Append("TargetFramework", "net8.0");
            Append("ImplicitUsings", _project.bImplicitUsings ? "enable" : "disable");
            Append("Nullable", _project.bNullable ? "enable" : "disable");
            Append("AllowUnsafeBlocks", _project.bAllowUnsafeBlocks.ToString().ToLower());
            Append("Authors", _project.Authors);
            Append("Company", _project.Company);
            Append("AssemblyVersion", _project.AssemblyVersion);
            Append("FileVersion", _project.FileVersion);
            Append("NeutralLanguage", _project.NeutralLanguage);
            
            List<string> finalWarningsToErros = [.._project.WarningsAsErrors];
            if (_project.bStrongRestrictedNullable)
            {
                finalWarningsToErros.Add("CS8600;CS8601;CS8602;CS8603;CS8604;CS8609;CS8610;CS8614;CS8616;CS8618;CS8619;CS8622;CS8625");
            }
            finalWarningsToErros.Insert(0, "$(WarningsAsErrors)");
            Append("WarningsAsErrors", string.Join(';', finalWarningsToErros));
            Append("Configurations", "DebugGame;DebugGame Editor;Development;Development Editor;Shipping;Shipping Editor");
            Append("Platforms", "AnyCPU");
            Append("UnrealProjectDir", _unrealProjectDir);
            Append("ZSharpProjectDir", Path.Combine(_unrealProjectDir, "Intermediate/ZSharp/ProjectFiles"));
            Append("ZSharpDir", _zsharpPluginDir);
            Append("SourceDir", _project.SourceDir);
            
            projectNode.AppendChild(propertyGroupNode);
            return propertyGroupNode;
        }

        private XmlElement BuildPropertyGroupByConfiguration(string config, XmlDocument doc, XmlElement projectNode)
        {
            XmlElement propertyGroupNode = doc.CreateElement("PropertyGroup");
            propertyGroupNode.SetAttribute("Condition", $" '$(Configuration)' == '{config}' ");
            void Append(string childName, string childInnerText) => AppendSimpleChild(doc, propertyGroupNode, childName, childInnerText);
            Append("Optimize", config switch
            {
                "DebugGame" or "DebugGame Editor" => "false",
                _ => "true"
            });
            Append("DebugSymbols", config switch
            {
                "Shipping" or "Shipping Editor" => "false",
                _ => "true"
            });
            Append("DebugType", config switch
            {
                "Shipping" or "Shipping Editor" => "none",
                _ => "embedded"
            });
            
            projectNode.AppendChild(propertyGroupNode);
            return propertyGroupNode;
        }

        private XmlElement BuildItemGroupNode(XmlDocument doc, XmlElement projectNode)
        {
            XmlElement itemGroupNode = doc.CreateElement("ItemGroup");
            
            List<string> finalUsings = [.._project.Usings];
            finalUsings.ForEach(us =>
            {
                XmlElement usingNode = doc.CreateElement("Using");
                usingNode.SetAttribute("Include", us);
                itemGroupNode.AppendChild(usingNode);
            });
            
            Dictionary<string, string> finalAliases = new(_project.Aliases);
            if (_project.bUnrealStylePrimitiveTypeAliases)
            {
                finalAliases["System.Byte"] = "uint8";
                finalAliases["System.UInt16"] = "uint16";
                finalAliases["System.UInt32"] = "uint32";
                finalAliases["System.UInt64"] = "uint64";
                finalAliases["System.SByte"] = "int8";
                finalAliases["System.Int16"] = "int16";
                finalAliases["System.Int32"] = "int32";
                finalAliases["System.Int64"] = "int64";
            }

            foreach (var pair in finalAliases)
            {
                XmlElement aliasNode = doc.CreateElement("Using");
                aliasNode.SetAttribute("Include", pair.Key);
                aliasNode.SetAttribute("Alias", pair.Value);
                itemGroupNode.AppendChild(aliasNode);
            }
            
            projectNode.AppendChild(itemGroupNode);
            return itemGroupNode;
        }

        private XmlElement BuildReferenceNode(XmlDocument doc, XmlElement projectNode)
        {
            XmlElement itemGroupNode = doc.CreateElement("ItemGroup");

            {
                XmlElement includeSourceNode = doc.CreateElement("Compile");
                includeSourceNode.SetAttribute("Include", "$(SourceDir)/**/*.cs");
                itemGroupNode.AppendChild(includeSourceNode);
            }
            
            List<string> finalProjectReferences = [.._project.ProjectReferences];
            foreach (var reference in finalProjectReferences)
            {
                XmlElement referenceNode = doc.CreateElement("ProjectReference");
                referenceNode.SetAttribute("Include", $"$(ZSharpProjectDir)/{reference}/{reference}.csproj");
                itemGroupNode.AppendChild(referenceNode);
            }
            
            List<string> finalReferences = [.._project.References];
            foreach (var reference in finalReferences)
            {
                throw new NotImplementedException();
            }
            
            projectNode.AppendChild(itemGroupNode);
            return itemGroupNode;
        }

        private XmlElement BuildTargetNode(XmlDocument doc, XmlElement projectNode)
        {
            XmlElement targetNode = doc.CreateElement("Target");
            targetNode.SetAttribute("Name", "PostBuild");
            targetNode.SetAttribute("AfterTargets", "PostBuildEvent");

            XmlElement mkdirNode = doc.CreateElement("Exec");
            string outputDir = "$(UnrealProjectDir)/Binaries/Managed";
            mkdirNode.SetAttribute("Command", $"if not exist \"{outputDir}\" mkdir \"{outputDir}\"");
            targetNode.AppendChild(mkdirNode);

            XmlElement copyNode = doc.CreateElement("Exec");
            copyNode.SetAttribute("Command", $"copy \"$(TargetPath)\" \"{outputDir}/$(TargetFileName)\"");
            targetNode.AppendChild(copyNode);
            
            projectNode.AppendChild(targetNode);
            return targetNode;
        }

        private ProjectDefinition _project;
        private string _unrealProjectDir;
        private string _zsharpPluginDir;

    }

    private class SolutionFileBuilder
    {

        public const string KHeader = "Microsoft Visual Studio Solution File, Format Version 12.00";
        public const string KFolderGuid = "{2150E333-8FDC-42A3-9474-1A3956D46DE8}";
        public const string KCsprojGuid = "{FAE04EC0-301F-11D3-BF4B-00C04F79EFBC}";

        public Guid AddFolder(string name)
        {
            Guid guid = Guid.NewGuid();
            _folders[guid] = $"Project(\"{KFolderGuid}\") = \"{name}\", \"{name}\", \"{FormatGuid(ref guid)}\"\nEndProject";
            _folderName2Guid[name] = guid;
            return guid;
        }

        public Guid AddFolder(string name, Guid parentFolder)
        {
            Guid guid = AddFolder(name);
            _nestedProjects.Add($"{FormatGuid(ref guid)} = {FormatGuid(ref parentFolder)}");
            return guid;
        }

        public Guid AddProject(ProjectDefinition project)
        {
            string projectFileExtension = project.ProjectFileExtension;
            string projectGuid = projectFileExtension switch
            {
                ".csproj" => KCsprojGuid,
                _ => throw new InvalidOperationException($"Unknown project type: {projectFileExtension}")
            };
            Guid guid = Guid.NewGuid();
            _projects[guid] = $"Project(\"{projectGuid}\") = \"{project.Name}\", \"{project.ProjectFilePath}\", \"{FormatGuid(ref guid)}\"\nEndProject";

            string parentFolder = project.ParentFolder;
            if (!string.IsNullOrWhiteSpace(parentFolder))
            {
                if (!_folderName2Guid.TryGetValue(parentFolder, out var parentFolderGuid))
                {
                    throw new ArgumentOutOfRangeException($"Solution folder {parentFolder} does not exist.");
                }
                
                _postConfigs.Add($"{FormatGuid(ref guid)}.DebugGame|Any CPU.ActiveCfg = DebugGame|Any CPU");
                _postConfigs.Add($"{FormatGuid(ref guid)}.DebugGame|Any CPU.Build.0 = DebugGame|Any CPU");
                _postConfigs.Add($"{FormatGuid(ref guid)}.DebugGame Editor|Any CPU.ActiveCfg = DebugGame Editor|Any CPU");
                _postConfigs.Add($"{FormatGuid(ref guid)}.DebugGame Editor|Any CPU.Build.0 = DebugGame Editor|Any CPU");
                
                _postConfigs.Add($"{FormatGuid(ref guid)}.Development|Any CPU.ActiveCfg = Development|Any CPU");
                _postConfigs.Add($"{FormatGuid(ref guid)}.Development|Any CPU.Build.0 = Development|Any CPU");
                _postConfigs.Add($"{FormatGuid(ref guid)}.Development Editor|Any CPU.ActiveCfg = Development Editor|Any CPU");
                _postConfigs.Add($"{FormatGuid(ref guid)}.Development Editor|Any CPU.Build.0 = Development Editor|Any CPU");
                
                _postConfigs.Add($"{FormatGuid(ref guid)}.Shipping|Any CPU.ActiveCfg = Shipping|Any CPU");
                _postConfigs.Add($"{FormatGuid(ref guid)}.Shipping|Any CPU.Build.0 = Shipping|Any CPU");
                _postConfigs.Add($"{FormatGuid(ref guid)}.Shipping Editor|Any CPU.ActiveCfg = Shipping Editor|Any CPU");
                _postConfigs.Add($"{FormatGuid(ref guid)}.Shipping Editor|Any CPU.Build.0 = Shipping Editor|Any CPU");
                
                _nestedProjects.Add($"{FormatGuid(ref guid)} = {FormatGuid(ref parentFolderGuid)}");
            }
            
            return guid;
        }

        public override string ToString()
        {
            StringBuilder sb = new();
            sb.Append("\n");
            sb.Append(KHeader).Append("\n");

            foreach (var pair in _folders)
            {
                sb.Append(pair.Value).Append("\n");
            }

            foreach (var pair in _projects)
            {
                sb.Append(pair.Value).Append("\n");
            }

            sb.Append("Global");
            sb.Append(@"
	GlobalSection(SolutionConfigurationPlatforms) = preSolution
		DebugGame|Any CPU = DebugGame|Any CPU
		DebugGame Editor|Any CPU = DebugGame Editor|Any CPU
		Development|Any CPU = Development|Any CPU
		Development Editor|Any CPU = Development Editor|Any CPU
        Shipping|Any CPU = Shipping|Any CPU
        Shipping Editor|Any CPU = Shipping Editor|Any CPU
	EndGlobalSection
");

            sb.Append("\tGlobalSection(ProjectConfigurationPlatforms) = postSolution\n");
            foreach (var config in _postConfigs)
            {
                sb.Append($"\t\t{config}\n");
            }
            sb.Append("\tEndGlobalSection\n");

            sb.Append("\tGlobalSection(NestedProjects) = preSolution\n");
            foreach (var project in _nestedProjects)
            {
                sb.Append($"\t\t{project}\n");
            }
            sb.Append("\tEndGlobalSection\n");

            sb.Append("EndGlobal\n");

            return sb.ToString();
        }
        
        private static string FormatGuid(ref readonly Guid guid) => guid.ToString("B").ToUpper();

        private Dictionary<Guid, string> _folders = new();
        private Dictionary<string, Guid> _folderName2Guid = new();
        private Dictionary<Guid, string> _projects = new();
        private List<string> _nestedProjects = new();
        private List<string> _postConfigs = new();

    }

}


