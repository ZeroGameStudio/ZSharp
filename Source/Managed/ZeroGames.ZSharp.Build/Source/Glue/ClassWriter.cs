﻿// Copyright Zero Games. All Rights Reserved.

using System.Text;
using System.Text.RegularExpressions;
using ZeroGames.ZSharp.UnrealEngine.Core;

namespace ZeroGames.ZSharp.Build.Glue;

public class ClassWriter : IDisposable, IAsyncDisposable
{

	public ClassWriter(ExportedAssemblyRegistry registry, ExportedClass exportedClass, Stream stream)
	{
		_registry = registry;
		_exportedClass = exportedClass;
		_sw = new(stream, Encoding.UTF8);
	}

	public void Dispose()
	{
		_sw.Dispose();
	}

	public async ValueTask DisposeAsync()
	{
		await _sw.DisposeAsync();
	}

	public Task WriteAsync() => Task.Run(() =>
	{
		_sw.Write(
@$"// Copyright Zero Games. All Rights Reserved.

// THIS FILE IS GENERATED BY ZSHARP.
// DO NOT MODIFY DIRECTLY!

#region GENERATED CODE

using ZeroGames.ZSharp.Core;
using ZeroGames.ZSharp.UnrealEngine;
{_extraUsingBlock}

namespace {_exportedClass.Namespace};

[System.CodeDom.Compiler.GeneratedCode(""ZSharp"", ""0.0.1"")]
public partial class {_exportedClass.Name} : {_baseType}, IConjugate<{_exportedClass.Name}>
{{

{_classBody}

    public {_newIfSubclass}static {_exportedClass.Name} BuildConjugate(IntPtr unmanaged) => new(unmanaged);
	protected {_exportedClass.Name}(IntPtr unmanaged) : base(unmanaged){{}}

}}

#endregion


");
	});

	private string GetTypeModule(string type)
	{
		string[] split = type.Split('.');
		if (split.Length < 2)
		{
			return string.Empty;
		}

		return split[0];
	}
	
	private string GetTypeName(string type)
	{
		string[] split = type.Split('.');
		return split.Length switch
		{
			1 => split[0],
			2 => split[1],
			_ => throw new InvalidOperationException($"Wrong export type name [{type}]")
		};
	}

	private string _extraUsingBlock
	{
		get
		{
			List<string> relevantModules = [ "Core", "CoreUObject", "Engine" ];
			
			relevantModules.Add(GetTypeModule(_baseType));
			
			relevantModules.RemoveAll(module => string.IsNullOrWhiteSpace(module) || module == _exportedClass.Module);

			return string.Join('\n', relevantModules.Distinct().Select(module => $"using {_registry.GetModuleAssembly(module)?.Name ?? throw new InvalidOperationException($"Unmapped module {module}")}.{module};"));
		}
	}

	private bool _topLevelClass => _exportedClass.BaseType[0] == '@';
	private string _newIfSubclass => _topLevelClass ? string.Empty : "new ";

	private string _baseType
	{
		get
		{
			if (_exportedClass.BaseType[0] == '@')
			{
				return _exportedClass.BaseType switch
				{
					"@UCLASS" => "UnrealClassExportedObjectBase",
					"@USTRUCT" => "UnrealStructExportedObjectBase",
					"@PLAIN" => "PlainUnmanagedClassExportedObjectBase",
					_ => throw new ArgumentException($"Invalid special base type {_exportedClass.BaseType}")
				};
			}

			return GetTypeName(_exportedClass.BaseType);
		}
	}

	private string _classBody
	{
		get
		{
			return string.Empty;
		}
	}

	private ExportedAssemblyRegistry _registry;
	private ExportedClass _exportedClass;
	private StreamWriter _sw;
	
}

