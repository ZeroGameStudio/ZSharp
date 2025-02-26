// Copyright Zero Games. All Rights Reserved.

#pragma warning disable RS1035 // Do not use banned APIs for analyzers

using System.Collections.Immutable;
using System.Text;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.CSharp.Syntax;
using Microsoft.CodeAnalysis.Text;
using ZeroGames.ZSharp.CodeDom.CSharp;

namespace ZeroGames.ZSharp.Emit.SourceGenerator.CSharp;

public class UStructSyntaxReceiver : ISyntaxContextReceiver
{
	
	public void OnVisitSyntaxNode(GeneratorSyntaxContext context)
	{
		if (context.Node is ClassDeclarationSyntax { AttributeLists.Count: > 0 } classDeclarationSyntax)
		{
			var typeSymbol = context.SemanticModel.GetDeclaredSymbol(classDeclarationSyntax) as ITypeSymbol;
			if (typeSymbol?.GetAttributes().Any(attr => attr.AttributeClass?.ToDisplayString() is "ZeroGames.ZSharp.Emit.Specifier.UStructAttribute") ?? false)
			{
				if (!_ustructSymbols.Contains(typeSymbol))
				{
					_ustructSymbols.Add(typeSymbol);
				}
			}
		}
	}
	
	public IReadOnlyList<ITypeSymbol> UStructSymbols => _ustructSymbols;

	private readonly List<ITypeSymbol> _ustructSymbols = [];
	
}

[Generator]
public class UStructGenerator : ISourceGenerator
{

	public void Initialize(GeneratorInitializationContext context)
	{
		context.RegisterForSyntaxNotifications(() => new UStructSyntaxReceiver());
	}

	public void Execute(GeneratorExecutionContext context)
	{
		if (context.SyntaxContextReceiver is not UStructSyntaxReceiver receiver)
		{
			return;
		}
		
		foreach (var uclassSymbol in receiver.UStructSymbols)
		{
			GenerateUStruct(uclassSymbol, context);
		}
	}

	private void GenerateUStruct(ITypeSymbol ustructSymbol, GeneratorExecutionContext context)
	{
		INamedTypeSymbol upropertySymbol = context.Compilation.GetTypeByMetadataName("ZeroGames.ZSharp.Emit.Specifier.UPropertyAttribute")!;

		string className = ustructSymbol.Name;
		string namespaceName = EmitGeneratorHelper.GetTypeNamespace(ustructSymbol);

		EmittedScriptStructBuilder builder = new(namespaceName, className);
		List<string> usings = new();

		var properties = ustructSymbol.GetMembers()
			.OfType<IPropertySymbol>()
			.Where(property => property.GetAttributes().Any(attr => SymbolEqualityComparer.Default.Equals(attr.AttributeClass, upropertySymbol)));

		foreach (var property in properties)
		{
			usings.Add(EmitGeneratorHelper.GetTypeNamespace(property.Type));
			builder.AddProperty
			(
				EmitGeneratorHelper.AccessibilityToMemberVisibility(property.DeclaredAccessibility),
				EmitGeneratorHelper.GetTypeReference(property.Type),
				property.Name
			);
		}

		usings.Remove(namespaceName);
		foreach (var us in usings)
		{
			builder.AddUsingNamespace(us);
		}
		
		CompilationUnit compilationUnit = builder.Build();
		CSharpGenerator generator = new();
		string content = generator.Generate(compilationUnit);
		
		context.AddSource($"{className}.g.cs", SourceText.From(content, Encoding.UTF8));
	}

}


