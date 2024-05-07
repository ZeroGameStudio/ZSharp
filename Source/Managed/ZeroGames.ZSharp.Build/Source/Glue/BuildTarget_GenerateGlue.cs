// Copyright Zero Games. All Rights Reserved.

using System.Text;
using System.Text.Json;

namespace ZeroGames.ZSharp.Build.Glue;

[BuildTarget("Glue")]
public class BuildTarget_GenerateGlue : BuildTargetBase, IUnrealProjectDir
{

	public override async Task<string> BuildAsync()
	{
		SetupTestData();
		
		await SetupRegistry();
		await Parallel.ForEachAsync(_registry.ExportedTypes, (type, _) => GenerateType(type));

		return "success";
	}
	
	public string UnrealProjectDir { get; }
	
	[FactoryConstructor]
	private BuildTarget_GenerateGlue(IBuildEngine engine, [Argument("projectdir")] string projectDir) : base(engine)
	{
		UnrealProjectDir = projectDir;
		if (!((IUnrealProjectDir)this).Valid)
		{
			throw new ArgumentException($"Invalid argument projectdir={projectDir}.");
		}
		_glueDir = $"{projectDir}/Intermediate/ZSharp/Glue";
	}

	private void SetupTestData()
	{
		using FileStream f = File.Create($"{_glueDir}/Game/Manifest.json");
		using StreamWriter sw = new(f, Encoding.UTF8);
		sw.Write(
$@"{{
	""Enums"":
	[
		{{
			""Name"": ""DamageType"",
			""Module"": ""ZHotel"",
			""UnderlyingType"": ""uint8"",
			""ValueMap"":
			{{
				""None"": ""0"",
				""Physical"": ""1"",
				""Mana"": ""2"",
				""Absolute"": ""3""
			}}
		}}
	]
}}
");
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
		ExportedAssembly? assembly = await JsonSerializer.DeserializeAsync<ExportedAssembly>(fs);
		if (assembly is null)
		{
			return;
		}

		assembly.Name = new DirectoryInfo(dir).Name;
		foreach (var exportedEnum in assembly.Enums)
		{
			exportedEnum.Assembly = assembly;
		}
		
		_registry.RegisterAssembly(assembly);

		string codeDir = $"{dir}/Glue";
		if (Directory.Exists(codeDir))
		{
			Directory.Delete(codeDir, true);
		}

		Directory.CreateDirectory(codeDir);
	}

	private async ValueTask GenerateType(ExportedType exportedType)
	{
		if (exportedType is ExportedClass exportedClass)
		{
			await GenerateClass(exportedClass);
		}
		else if (exportedType is ExportedEnum exportedEnum)
		{
			await GenerateEnum(exportedEnum);
		}
	}

	private ValueTask GenerateClass(ExportedClass exportedClass)
	{
		return ValueTask.CompletedTask;
	}

	private async ValueTask GenerateEnum(ExportedEnum exportedEnum)
	{
		await using FileStream fs = File.Create($"{_glueDir}/{exportedEnum.Assembly.Name}/Glue/{exportedEnum.Name}.cs");
		await using EnumWriter ew = new(exportedEnum, fs);
		await ew.WriteAsync();
	}
	
	private ExportedAssemblyRegistry _registry = new();
	private string _glueDir;
}


