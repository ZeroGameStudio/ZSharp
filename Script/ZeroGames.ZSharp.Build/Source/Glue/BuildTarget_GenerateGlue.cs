// Copyright Zero Games. All Rights Reserved.

using System.Text.Json;

namespace ZeroGames.ZSharp.Build.Glue;

[BuildTarget("Glue")]
public class BuildTarget_GenerateGlue : BuildTargetBase, IUnrealProjectDir
{

	public override async Task<string> BuildAsync()
	{
		await SetupRegistry();
		Parallel.ForEach(_registry.ExportedTypes, (type, _) => GenerateType(type));

		GenerateMisc();

		return "success";
	}
	
	public string UnrealProjectDir { get; }
	
	[FactoryConstructor]
	private BuildTarget_GenerateGlue(IBuildEngine engine, [Argument("projectdir")] string projectDir, [Argument("assemblies")] string? assemblies = null) : base(engine)
	{
		UnrealProjectDir = projectDir;
		if (!((IUnrealProjectDir)this).IsValid)
		{
			throw new ArgumentException($"Invalid argument projectdir={projectDir}.");
		}

		_assemblies = assemblies?.Split(',');
		
		_glueDir = $"{projectDir}/Intermediate/ZSharp/Glue";
	}

	private async Task SetupRegistry()
	{
		List<string> assemblyPaths = GatherAssemblies();
		await Parallel.ForEachAsync(assemblyPaths, (path, _) => GenerateAssembly(path));
		_registry.FinishRegister();
	}

	private List<string> GatherAssemblies()
	{
		List<string> res = new();
		
		foreach (var dir in Directory.GetDirectories(_glueDir))
		{
			string dirName = new DirectoryInfo(dir).Name;
			
			// Skip preserved dirs.
			if (dirName.StartsWith("__"))
			{
				continue;
			}
			
			if (!File.Exists($"{dir}/Manifest.json"))
			{
				continue;
			}
			
			if (_assemblies?.Contains(dirName) is false)
			{
				continue;
			}
			
			res.Add(dir);
		}

		return res;
	}

	private async ValueTask GenerateAssembly(string dir)
	{
		await using FileStream fs = File.OpenRead($"{dir}/Manifest.json");
		JsonSerializerOptions options = new()
		{
			PropertyNameCaseInsensitive = true,
		};
		ExportedAssembly? assembly = await JsonSerializer.DeserializeAsync<ExportedAssembly>(fs, options);
		if (assembly is null)
		{
			return;
		}

		assembly.Name = new DirectoryInfo(dir).Name;
		foreach (var type in assembly.ExportedTypes)
		{
			type.Assembly = assembly.Name;
		}
		
		_registry.RegisterAssembly(assembly);

		string codeDir = $"{dir}/Glue";
		lock (_codeDirLock)
		{
			if (Directory.Exists(codeDir))
			{
				Directory.Delete(codeDir, true);
			}

			Directory.CreateDirectory(codeDir);
		}
	}

	private void GenerateType(ExportedType exportedType)
	{
		string moduleDir = $"{_glueDir}/{exportedType.Assembly}/Glue/{exportedType.Module}";
		CreateModuleDirectory(moduleDir);
		
		if (exportedType is ExportedClass exportedClass)
		{
			new ClassWriter(_registry, exportedClass, moduleDir, exportedType.Name.Substring(1)).Write();
		}
		else if (exportedType is ExportedEnum exportedEnum)
		{
			new EnumWriter(_registry, exportedEnum, moduleDir, exportedType.Name).Write();
		}
		else if (exportedType is ExportedDelegate exportedDelegate)
		{
			new DelegateWriter(_registry, exportedDelegate, moduleDir, exportedType.Name.Substring(1)).Write();
		}
	}
	
	private void GenerateMisc()
	{
		string miscDir = $"{_glueDir}/__Misc";
		string manifestDir = $"{miscDir}/Manifest.json";
		if (!File.Exists(manifestDir))
		{
			return;
		}

		string codeDir = $"{miscDir}/Glue";
		if (Directory.Exists(codeDir))
		{
			Directory.Delete(codeDir, true);
		}
		
		Directory.CreateDirectory(codeDir);

		using FileStream fs = File.OpenRead(manifestDir);
		JsonSerializerOptions options = new()
		{
			PropertyNameCaseInsensitive = true,
		};
		if (JsonSerializer.Deserialize<ExportedMisc>(fs, options) is not { } misc)
		{
			return;
		}
		
		new MiscWriter(misc, codeDir).Write();
	}

	private void CreateModuleDirectory(string dir)
	{
		lock (_moduleDirLock)
		{
			if (!Directory.Exists(dir))
			{
				Directory.CreateDirectory(dir);
			}
		}
	}

	private string[]? _assemblies;
	
	private readonly ExportedAssemblyRegistry _registry = new();
	private string _glueDir;
	private readonly Lock _codeDirLock = new();
	private readonly Lock _moduleDirLock = new();
}


