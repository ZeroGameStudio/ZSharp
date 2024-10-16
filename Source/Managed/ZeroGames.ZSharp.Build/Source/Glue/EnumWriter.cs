// Copyright Zero Games. All Rights Reserved.

using System.Text;
using ZeroGames.ZSharp.CodeDom.CSharp;

namespace ZeroGames.ZSharp.Build.Glue;

public class EnumWriter : IDisposable
{

	public EnumWriter(ExportedAssemblyRegistry registry, ExportedEnum exportedEnum, Stream stream)
	{
		_exportedEnum = exportedEnum;
		_sw = new(stream, Encoding.UTF8);
	}

	~EnumWriter()
	{
		InternalDispose();
	}

	public void Dispose()
	{
		InternalDispose();
		GC.SuppressFinalize(this);
	}

	public void Write()
	{
		ExportedEnumBuilder builder = new(_exportedEnum.Namespace, _exportedEnum.Name)
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

		_sw.Write(content);
	}

	private void InternalDispose()
	{
		if (_disposed)
		{
			return;
		}

		_disposed = true;
		
		_sw.Dispose();
	}

	private ExportedEnum _exportedEnum;
	private StreamWriter _sw;

	private bool _disposed;

}


