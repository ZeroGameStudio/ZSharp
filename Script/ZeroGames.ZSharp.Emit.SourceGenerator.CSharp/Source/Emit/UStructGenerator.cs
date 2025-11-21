// Copyright Zero Games. All Rights Reserved.

#pragma warning disable RS1035 // Do not use banned APIs for analyzers

using System.Text;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.Text;
using ZeroGames.ZSharp.CodeDom.CSharp;

namespace ZeroGames.ZSharp.Emit.SourceGenerator.CSharp;

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
		
		foreach (var pair in receiver.SymbolMap)
		{
			GenerateUStruct(pair.Key, context, pair.Value);
		}
	}
	
	private class UStructSyntaxReceiver : UFieldSyntaxReceiverBase
	{
		protected override string FieldSpecifierName => "UStruct";
	}

	private void GenerateUStruct(ITypeSymbol ustructSymbol, GeneratorExecutionContext context, bool implicitBase)
	{
		INamedTypeSymbol upropertySymbol = context.Compilation.GetTypeByMetadataName("ZeroGames.ZSharp.Emit.Specifier.UPropertyAttribute")!;

		string className = ustructSymbol.Name;
		string namespaceName = EmitGeneratorHelper.GetTypeNamespace(ustructSymbol);

		EmittedScriptStructBuilder builder = new(namespaceName, className, implicitBase);
		HashSet<string> usings = ["System.Runtime.InteropServices"];

		var properties = ustructSymbol.GetMembers()
			.OfType<IPropertySymbol>()
			.Where(property => property.GetAttributes().Any(attr => SymbolEqualityComparer.Default.Equals(attr.AttributeClass, upropertySymbol)));

		foreach (var property in properties)
		{
			EmitGeneratorHelper.LootNamespace(property.Type, usings);
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
		
		context.AddSource($"{className.Substring(1)}.g.cs", SourceText.From(content, Encoding.UTF8));
	}

}


