// Copyright Zero Games. All Rights Reserved.

using System.Text;

namespace ZeroGames.ZSharp.Build.Solution;

public class SolutionFileBuilder
{
	public Guid AddFolder(string path)
	{
		Guid guid = Guid.NewGuid();
		string name = path.Split('/').Last();
		_folders[guid] = $"Project(\"{FOLDER_GUID}\") = \"{name}\", \"{name}\", \"{FormatGuid(ref guid)}\"\nEndProject";
		_folderPath2Guid[path] = guid;
		return guid;
	}

	public Guid AddFolder(string path, string parentFolder)
	{
		Guid guid = AddFolder(path);
		Guid parentGuid = _folderPath2Guid[parentFolder];
		_nestedProjects.Add($"{FormatGuid(ref guid)} = {FormatGuid(ref parentGuid)}");
		return guid;
	}

	public Guid AddProject(ProjectDefinition project)
	{
		string projectFileExtension = project.ProjectFileExtension;
		string projectGuid = projectFileExtension switch
		{
			".csproj" => CSPROJ_GUID,
			".vbproj" => throw new NotImplementedException(),
			_ => throw new InvalidOperationException($"Unknown project type: {projectFileExtension}")
		};
		Guid guid = Guid.NewGuid();
		_projects[guid] = $"Project(\"{projectGuid}\") = \"{project.Name}\", \"{project.ProjectFilePath}\", \"{FormatGuid(ref guid)}\"\nEndProject";

		string parentFolder = project.Folder;
		if (!string.IsNullOrWhiteSpace(parentFolder))
		{
			if (!_folderPath2Guid.TryGetValue(parentFolder, out var parentFolderGuid))
			{
				throw new ArgumentOutOfRangeException($"Solution folder {parentFolder} does not exist.");
			}

			_postConfigs.Add($"{FormatGuid(ref guid)}.DebugGame|Any CPU.ActiveCfg = DebugGame|Any CPU");
			_postConfigs.Add($"{FormatGuid(ref guid)}.DebugGame|Any CPU.Build.0 = DebugGame|Any CPU");

			_postConfigs.Add($"{FormatGuid(ref guid)}.Development|Any CPU.ActiveCfg = Development|Any CPU");
			_postConfigs.Add($"{FormatGuid(ref guid)}.Development|Any CPU.Build.0 = Development|Any CPU");

			_postConfigs.Add($"{FormatGuid(ref guid)}.Shipping|Any CPU.ActiveCfg = Shipping|Any CPU");
			_postConfigs.Add($"{FormatGuid(ref guid)}.Shipping|Any CPU.Build.0 = Shipping|Any CPU");

			_nestedProjects.Add($"{FormatGuid(ref guid)} = {FormatGuid(ref parentFolderGuid)}");
		}

		return guid;
	}

	public override string ToString()
	{
		StringBuilder sb = new();
		sb.Append("\n");
		sb.Append(HEADER).Append("\n");

		foreach (var pair in _folders)
		{
			sb.Append(pair.Value).Append("\n");
		}

		foreach (var pair in _projects)
		{
			sb.Append(pair.Value).Append("\n");
		}

		sb.Append(
@"Global
	GlobalSection(SolutionConfigurationPlatforms) = preSolution
		DebugGame|Any CPU = DebugGame|Any CPU
		Development|Any CPU = Development|Any CPU
        Shipping|Any CPU = Shipping|Any CPU
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
	
	private const string HEADER = "Microsoft Visual Studio Solution File, Format Version 12.00";
	private const string FOLDER_GUID = "{2150E333-8FDC-42A3-9474-1A3956D46DE8}";
	private const string CSPROJ_GUID = "{FAE04EC0-301F-11D3-BF4B-00C04F79EFBC}";

	private Dictionary<Guid, string> _folders = new();
	private Dictionary<string, Guid> _folderPath2Guid = new();
	private Dictionary<Guid, string> _projects = new();
	private List<string> _nestedProjects = new();
	private List<string> _postConfigs = new();
}


