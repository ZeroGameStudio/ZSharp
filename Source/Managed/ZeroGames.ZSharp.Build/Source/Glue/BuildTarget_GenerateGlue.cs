// Copyright Zero Games. All Rights Reserved.

using System.Text.Json;

namespace ZeroGames.ZSharp.Build.Glue;

[BuildTarget("Glue")]
public class BuildTarget_GenerateGlue : BuildTargetBase, IUnrealProjectDir
{

	public override async Task<string> BuildAsync()
	{
		// SetupTestData();
		
		await SetupRegistry();
		await Parallel.ForEachAsync(_registry.ExportedTypes, (type, _) => GenerateType(type));

		return "success";
	}
	
	public string UnrealProjectDir { get; }
	
	[FactoryConstructor]
	private BuildTarget_GenerateGlue(IBuildEngine engine, [Argument("projectdir")] string projectDir) : base(engine)
	{
		UnrealProjectDir = projectDir;
		if (!((IUnrealProjectDir)this).IsValid)
		{
			throw new ArgumentException($"Invalid argument projectdir={projectDir}.");
		}
		_glueDir = $"{projectDir}/Intermediate/ZSharp/Glue";
	}
	
	private void SetupTestData()
	{
		File.Copy($"{_glueDir}/../../../Content/Manifest.json", $"{_glueDir}/Game/Manifest.json", true);
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
			if (!File.Exists($"{dir}/Manifest.json"))
			{
				continue;
			}
			
			if (File.Exists($"{dir}/.timestamp"))
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

	private async ValueTask GenerateType(ExportedType exportedType)
	{
		string moduleDir = $"{_glueDir}/{exportedType.Assembly}/Glue/{exportedType.Module}";
		CreateModuleDirectory(moduleDir);
		
		await using FileStream fs = File.Create($"{moduleDir}/{exportedType.Name}.g.cs");
		if (exportedType is ExportedClass exportedClass)
		{
			await using ClassWriter cw = new(_registry, exportedClass, fs);
			await cw.WriteAsync();
		}
		else if (exportedType is ExportedEnum exportedEnum)
		{
			await using EnumWriter ew = new(exportedEnum, fs);
			await ew.WriteAsync();
		}
		else if (exportedType is ExportedDelegate exportedDelegate)
		{
			await using DelegateWriter dw = new(_registry, exportedDelegate, fs);
			await dw.WriteAsync();
		}
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
	
	private ExportedAssemblyRegistry _registry = new();
	private string _glueDir;
	private object _codeDirLock = new();
	private object _moduleDirLock = new();
}


