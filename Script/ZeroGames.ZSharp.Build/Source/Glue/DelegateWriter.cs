// Copyright Zero Games. All Rights Reserved.

using System.Text;
using ZeroGames.ZSharp.CodeDom.CSharp;

namespace ZeroGames.ZSharp.Build.Glue;

public class DelegateWriter
{

	public DelegateWriter(ExportedAssemblyRegistry registry, ExportedDelegate exportedDelegate, string directory, string fileName)
	{
		_exportedDelegate = exportedDelegate;
		_filePath = $"{directory}/{fileName}.cs";
	}

	public void Write()
	{
		string delegateName = _exportedDelegate.Name.Split('.')[^1];
		ExportedDelegateBuilder builder = new(_exportedDelegate.Namespace, delegateName, _exportedDelegate.UnrealFieldPath, _exportedDelegate.IsSparse ? EDelegateKind.Sparse : _exportedDelegate.IsMulticast ? EDelegateKind.Multicast : EDelegateKind.Unicast, _exportedDelegate.ConjugateKey);
		var usings = NamespaceHelper.LootNamespace(_exportedDelegate).Where(ns => ns != _exportedDelegate.Namespace);
		foreach (var ns in usings)
		{
			builder.AddUsingNamespace(ns);
		}
		
		builder.ReturnType = _exportedDelegate.ReturnParameter?.Type.ToString() is {} returnTypeName ? new(returnTypeName, _exportedDelegate.ReturnParameter.UnderlyingType, _exportedDelegate.ReturnParameter.IsNullInNotNullOut) : null;

		List<ParameterDeclaration> parameters = new();
		foreach (var parameter in _exportedDelegate.Parameters)
		{
			if (parameter.IsReturn)
			{
				break;
			}
			
			EParameterKind kind = parameter.IsInOut ? EParameterKind.Ref : parameter.IsOut ? EParameterKind.Out : EParameterKind.In;
			AttributeDeclaration[]? attributes = parameter is { IsNullInNotNullOut: true, IsInOut: true } ? [ new("NotNull") ] : null;
			parameters.Add(new(kind, new(parameter.Type.ToString(), parameter.UnderlyingType, parameter.IsNullInNotNullOut), parameter.Name, attributes));
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

	private readonly ExportedDelegate _exportedDelegate;
	private readonly string _filePath;

}