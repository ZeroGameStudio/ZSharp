// Copyright Zero Games. All Rights Reserved.

#pragma warning disable RS1035 // Do not use banned APIs for analyzers

using System.Text;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.Text;
using ZeroGames.ZSharp.CodeDom.CSharp;

namespace ZeroGames.ZSharp.Emit.SourceGenerator.CSharp;

[Generator]
public class UDelegateGenerator : ISourceGenerator
{

	public void Initialize(GeneratorInitializationContext context)
	{
		context.RegisterForSyntaxNotifications(() => new UDelegateSyntaxReceiver());
	}

	public void Execute(GeneratorExecutionContext context)
	{
		if (context.SyntaxContextReceiver is not UDelegateSyntaxReceiver receiver)
		{
			return;
		}
		
		foreach (var pair in receiver.SymbolMap)
		{
			GenerateUDelegate(pair.Key, context);
		}
	}
	
	private class UDelegateSyntaxReceiver : UFieldSyntaxReceiverBase
	{
		protected override string FieldSpecifierName => "UDelegate";
	}

	private void GenerateUDelegate(ITypeSymbol udelegateSymbol, GeneratorExecutionContext context)
	{
		string className = udelegateSymbol.Name;
		string namespaceName = EmitGeneratorHelper.GetTypeNamespace(udelegateSymbol);
		
		ITypeSymbol? outerTypeSymbol = udelegateSymbol.ContainingType;
		string? outerTypeName = outerTypeSymbol?.Name;
		
		INamedTypeSymbol? baseType = udelegateSymbol.BaseType;
		if (baseType is null)
		{
			return;
		}

		string baseTypeFullName = baseType.ToDisplayString();
		EDelegateKind kind = baseTypeFullName switch
		{
			"ZeroGames.ZSharp.UnrealEngine.CoreUObject.UnrealDelegateBase" => EDelegateKind.Unicast,
			"ZeroGames.ZSharp.UnrealEngine.CoreUObject.UnrealMulticastInlineDelegateBase" => EDelegateKind.Multicast,
			"ZeroGames.ZSharp.UnrealEngine.CoreUObject.UnrealMulticastSparseDelegateBase" => EDelegateKind.Sparse,
			_ => throw new NotSupportedException($"{baseTypeFullName}")
		};

		EmittedDelegateBuilder builder = new(namespaceName, className, outerTypeName, kind);
		List<string> usings = new();
		
		ITypeSymbol signatureTypeSymbol = udelegateSymbol.GetTypeMembers().Single(t => t.Name is "Signature");
		IMethodSymbol signature = signatureTypeSymbol.GetMembers().OfType<IMethodSymbol>().Single(m => m.Name == "Invoke");
		builder.ReturnType = !signature.ReturnsVoid ? EmitGeneratorHelper.GetTypeReference(signature.ReturnType) : null;
		if (!signature.ReturnsVoid)
		{
			usings.Add(EmitGeneratorHelper.GetTypeNamespace(signature.ReturnType));
		}
			
		List<ParameterDeclaration> parameters = new();
		foreach (var parameter in signature.Parameters)
		{
			usings.Add(EmitGeneratorHelper.GetTypeNamespace(parameter.Type));
				
			parameters.Add(new(EmitGeneratorHelper.RefKindToParameterKind(parameter.RefKind), EmitGeneratorHelper.GetTypeReference(parameter.Type), parameter.Name));
		}

		builder.Parameters = parameters.ToArray();

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


