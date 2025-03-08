// Copyright Zero Games. All Rights Reserved.

using System.Text;
using ZeroGames.ZSharp.CodeDom.CSharp;

namespace ZeroGames.ZSharp.Build.Glue;

public class EnumWriter
{

	public EnumWriter(ExportedAssemblyRegistry registry, ExportedEnum exportedEnum, string directory, string fileName)
	{
		_exportedEnum = exportedEnum;
		_filePath = $"{directory}/{fileName}.cs";
	}

	public void Write()
	{
		ExportedEnumBuilder builder = new(_exportedEnum.Namespace, _exportedEnum.Name, _exportedEnum.UnrealFieldPath)
		{
			UnderlyingType = _exportedEnum.UnderlyingType,
			IsFlags = _exportedEnum.IsFlags,
		};
		foreach (var pair in _exportedEnum.ValueMap)
		{
			builder.AddMember(pair.Key, pair.Value);
		}

		CompilationUnit compilationUnit = builder.Build();
		CSharpGenerator generator = new();
		string content = generator.Generate(compilationUnit);

		using StreamWriter sw = new(File.Create(_filePath), Encoding.UTF8);
		sw.Write(content);
	}

	private ExportedEnum _exportedEnum;
	private string _filePath;

}


