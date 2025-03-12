// Copyright Zero Games. All Rights Reserved.

using System.Collections.Concurrent;
using System.Text.Json;

namespace ZeroGames.ZSharp.Build.Solution;

[BuildTarget("Solution")]
public class BuildTarget_GenerateSolution : BuildTargetBase, IUnrealProjectDir
{
	
	public override async Task<string> BuildAsync()
	{
		PrepareDirectory();

		ConcurrentQueue<object> messages = [];
		void Log(object message) => messages.Enqueue(message);

		Task solutionTask = new SolutionFileWriter(_manifest.Solution, Log).WriteAsync(CancellationToken.None);
		Task projectTask = Parallel.ForEachAsync(_manifest.Projects, (project, cancellationToken) => new ProjectFileWriter(UnrealProjectDir, ZSharpPluginDir, project, Log).WriteAsync(cancellationToken));
		await Task.WhenAll(solutionTask, projectTask);
		
		return string.Join(Environment.NewLine, messages);
	}
	
	public string UnrealProjectDir { get; }
	public string ZSharpPluginDir { get; }
	
	private class ModuleMappingInfoDto
	{
		public required Dictionary<string, string> Mapping { get; set; }
	}

	[FactoryConstructor]
	private BuildTarget_GenerateSolution(IBuildEngine engine
		, [Argument("projectdir")] string projectDir
		, [Argument("zsharpdir")] string zsharpDir
		, [Argument("source")] string source
		, [Argument("modulemappinginfo")] string moduleMappingInfo) : base(engine)
	{
		UnrealProjectDir = projectDir.TrimEnd('/', '\\');
		ZSharpPluginDir = zsharpDir.TrimEnd('/', '\\');
		
		string[] sources = source.Split(';');
		
		JsonSerializerOptions options = new()
		{
			PropertyNameCaseInsensitive = true,
		};
		var moduleMappingInfoDto = JsonSerializer.Deserialize<ModuleMappingInfoDto>(moduleMappingInfo, options);
		if (moduleMappingInfoDto is null)
		{
			throw new ArgumentException($"Invalid argument modulemappinginfo={moduleMappingInfo}.");
		}

		Dictionary<string, string> moduleMap = moduleMappingInfoDto.Mapping;

		_manifest = new(UnrealProjectDir, ZSharpPluginDir, sources, moduleMap);
	}

	private void PrepareDirectory()
	{
		string currentDir = UnrealProjectDir;
		if (!Directory.Exists(currentDir))
		{
			throw new FileNotFoundException($"Project dir {UnrealProjectDir} not exists.");
		}

		currentDir += "/Intermediate";
		if (!Directory.Exists(currentDir))
		{
			Directory.CreateDirectory(currentDir);
		}

		currentDir += "/ZSharp";
		if (!Directory.Exists(currentDir))
		{
			Directory.CreateDirectory(currentDir);
		}
		
		currentDir += "/ProjectFiles";
		if (Directory.Exists(currentDir))
		{
			Directory.Delete(currentDir, true);
		}
		
		Directory.CreateDirectory(currentDir);
	}

	private ProjectManifest _manifest;

}


