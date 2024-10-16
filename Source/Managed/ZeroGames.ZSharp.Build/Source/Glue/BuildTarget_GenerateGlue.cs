// Copyright Zero Games. All Rights Reserved.

using System.Text.Json;

namespace ZeroGames.ZSharp.Build.Glue;

[BuildTarget("Glue")]
public class BuildTarget_GenerateGlue : BuildTargetBase, IUnrealProjectDir
{

	public override async Task<string> BuildAsync()
	{
		await SetupRegistry();
		await Parallel.ForEachAsync(_registry.ExportedTypes, (type, _) => GenerateType(type));

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
			if (!File.Exists($"{dir}/Manifest.json"))
			{
				continue;
			}
			
			if (!(_assemblies?.Contains(new DirectoryInfo(dir).Name) ?? true))
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
		
		if (exportedType is ExportedClass exportedClass)
		{
			await using FileStream fs = File.Create($"{moduleDir}/{exportedType.Name}.g.cs");
			await using ClassWriter cw = new(_registry, exportedClass, fs);
			await cw.WriteAsync();
		}
		else if (exportedType is ExportedEnum exportedEnum)
		{
			using EnumWriter ew = new(_registry, exportedEnum, moduleDir, exportedType.Name);
			ew.Write();
		}
		else if (exportedType is ExportedDelegate exportedDelegate)
		{
			using DelegateWriter dw = new(_registry, exportedDelegate, moduleDir, exportedType.Name);
			dw.Write();
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

	private string[]? _assemblies;
	
	private ExportedAssemblyRegistry _registry = new();
	private string _glueDir;
	private object _codeDirLock = new();
	private object _moduleDirLock = new();
}


