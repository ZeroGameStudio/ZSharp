// Copyright Zero Games. All Rights Reserved.

using System.Text;

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public class CSharpGenerator
{

	public string Generate(CompilationUnit compilationUnit) => GenerateCompilationUnit(compilationUnit);

	private string GenerateCompilationUnit(CompilationUnit compilationUnit)
	{
		StringBuilder sb = new();

		// Header blocks
		if (compilationUnit.HeaderBlocks.Count > 0)
		{
			sb.AppendLine(string.Join(Environment.NewLine, compilationUnit.HeaderBlocks.Select(GenerateBlock)));
		}
		
		// Usings
		if (compilationUnit.Usings.Count > 0)
		{
			sb.AppendLine();
			sb.AppendLine(string.Join(Environment.NewLine, compilationUnit.Usings.Select(GenerateUsingStatement)));
		}
		
		// Namespace
		if (compilationUnit.Namespace is not null)
		{
			sb.AppendLine();
			sb.AppendLine(GenerateNamespaceStatement(compilationUnit.Namespace.Value));
		}
		
		// Types
		if (compilationUnit.Types.Count > 0)
		{
			sb.AppendLine();
			sb.AppendLine(string.Join(Environment.NewLine, compilationUnit.Types.Select(GenerateTypeDefinition)));
		}
		
		// End blocks
		if (compilationUnit.EndBlocks.Count > 0)
		{
			sb.AppendLine();
			sb.Append(string.Join(Environment.NewLine, compilationUnit.EndBlocks.Select(GenerateBlock)));
		}

		return sb.ToString();
	}

	private string GenerateBlock(Block block)
	{
		return _blockGenerator.Generate(block);
	}

	private string GenerateUsingStatement(UsingStatement usingStatement)
	{
		List<string> keywords = [ "using" ];
		if (usingStatement.IsGlobal)
		{
			keywords.Insert(0, "global");
		}

		if (usingStatement.IsStatic)
		{
			keywords.Add("static");
		}
		
		string alias = usingStatement.Alias is not null ? $"{usingStatement.Alias} = " : string.Empty;
		return $"{string.Join(" ", keywords)} {alias}{usingStatement.Target};";
	}

	private string GenerateNamespaceStatement(NamespaceStatement namespaceStatement)
	{
		return $"namespace {namespaceStatement.NamespaceName};";
	}

	private string GenerateTypeDefinition(TypeDefinitionBase typeDefinition)
	{
		if (typeDefinition is EnumDefinition enumDefinition)
		{
			return new EnumGenerator().Generate(enumDefinition);
		}

		throw new NotSupportedException();
	}

	private readonly BlockGenerator _blockGenerator = new();

}


