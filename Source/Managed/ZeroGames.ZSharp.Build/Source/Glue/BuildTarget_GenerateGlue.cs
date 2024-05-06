// Copyright Zero Games. All Rights Reserved.

using System.Text;
using System.Text.Json;

namespace ZeroGames.ZSharp.Build.Glue;

[BuildTarget("Glue")]
public class BuildTarget_GenerateGlue : BuildTargetBase
{

	public override async Task<string> BuildAsync()
	{
		{
			await using FileStream f = File.Create($"{_glueDir}/.glue");
			await using StreamWriter sw = new(f, Encoding.UTF8);
			sw.Write(
$@"{{
	""Enums"":
	[
		{{
			""Name"": ""DamageType"",
			""Module"": ""ZHotel"",
			""Assembly"": ""Game"",
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
		
		await using FileStream fs = File.OpenRead($"{_glueDir}/.glue");
		_registry = await JsonSerializer.DeserializeAsync<ExportedTypeRegistry>(fs);
		if (_registry is null)
		{
			return "no glue";
		}

		Task enumTask = Parallel.ForEachAsync(_registry.Enums, (exportedEnum, token) => GenerateEnum(exportedEnum));

		await Task.WhenAll([enumTask]);
		return "success";
	}
	
	[FactoryConstructor]
	private BuildTarget_GenerateGlue(IBuildEngine engine, [Argument("projectdir")] string projectDir) : base(engine)
	{
		_projectDir = projectDir;
		_glueDir = "D:/Projects/UE5/ZLab/Intermediate/ZSharp/Glue";
		_createDirectoryLock = new();
	}

	private async ValueTask GenerateEnum(ExportedEnum exportedEnum)
	{
		CreateDirectoryForAssemblyIfNotExists(exportedEnum.Assembly);
		await using FileStream fs = File.Create($"{_glueDir}/{exportedEnum.Assembly}/Glue/{exportedEnum.Name}.cs");
		await using EnumWriter ew = new(exportedEnum, fs);
		await ew.WriteAsync();
	}

	private void CreateDirectoryForAssemblyIfNotExists(string assembly)
	{
		lock (_createDirectoryLock)
		{
			string dir = $"{_glueDir}/{assembly}";
			string glueDir = $"{dir}/Glue";
			if (!Directory.Exists(dir))
			{
				Directory.CreateDirectory(dir);
				Directory.CreateDirectory(glueDir);
			}
			else if (!Directory.Exists(glueDir))
			{
				Directory.CreateDirectory(glueDir);
			}
		}
	}
	
	private ExportedTypeRegistry? _registry;
	private string _projectDir;
	private string _glueDir;
	private object _createDirectoryLock;

}


