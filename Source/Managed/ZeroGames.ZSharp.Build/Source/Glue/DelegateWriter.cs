// Copyright Zero Games. All Rights Reserved.

using System.Text;
using ZeroGames.ZSharp.CodeDom.CSharp;

namespace ZeroGames.ZSharp.Build.Glue;

public class DelegateWriter
{

	public DelegateWriter(ExportedAssemblyRegistry registry, ExportedDelegate exportedDelegate, string directory, string fileName)
	{
		_exportedDelegate = exportedDelegate;
		_filePath = $"{directory}/{fileName}.g.cs";
	}

	public void Write()
	{
		string delegateName = _exportedDelegate.Name.Split('.')[^1];
		ExportedDelegateBuilder builder = new(_exportedDelegate.Namespace, delegateName, _exportedDelegate.UnrealFieldPath, _exportedDelegate.IsSparse ? EExportedDelegateKind.Sparse : _exportedDelegate.IsMulticast ? EExportedDelegateKind.Multicast : EExportedDelegateKind.Unicast);
		var usings = NamespaceHelper.LootNamespace(_exportedDelegate).Where(ns => ns != _exportedDelegate.Namespace);
		foreach (var ns in usings)
		{
			builder.AddUsingNamespace(ns);
		}

		builder.ReturnType = _exportedDelegate.ReturnParameter?.Type.ToString() is {} typeName ? new(typeName, _exportedDelegate.ReturnParameter.UnderlyingType) : null;
		
		List<ParameterDeclaration> parameters = new();
		foreach (var parameter in _exportedDelegate.Parameters)
		{
			if (parameter.IsReturn)
			{
				break;
			}
			
			EParameterKind kind = parameter.IsInOut ? EParameterKind.Ref : parameter.IsOut ? EParameterKind.Out : EParameterKind.In;
			AttributeDeclaration[]? attributes = parameter.IsNullInNotNullOut ? [ new("NotNull") ] : null;
			parameters.Add(new(kind, new(parameter.Type.ToString(), parameter.UnderlyingType), parameter.Name, attributes));
		}
		builder.Parameters = parameters.ToArray();

		if (_exportedDelegate.Name.Contains('.'))
		{
			builder.OuterClassName = _exportedDelegate.Name.Split('.')[0];
		}

		CompilationUnit compilationUnit = builder.Build();
		CSharpGenerator generator = new();
		string content = generator.Generate(compilationUnit);

		using StreamWriter sw = new(File.Create(_filePath), Encoding.UTF8);
		sw.Write(content);
	}

	private ExportedDelegate _exportedDelegate;
	private string _filePath;

}