// Copyright Zero Games. All Rights Reserved.

using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp.Syntax;

namespace ZeroGames.ZSharp.Emit.SourceGenerator.CSharp;

public abstract class UFieldSyntaxReceiverBase : ISyntaxContextReceiver
{
	
	public void OnVisitSyntaxNode(GeneratorSyntaxContext context)
	{
		if (context.Node is ClassDeclarationSyntax { AttributeLists.Count: > 0 } classDeclarationSyntax)
		{
			var typeSymbol = context.SemanticModel.GetDeclaredSymbol(classDeclarationSyntax) as ITypeSymbol;
			if (typeSymbol?.GetAttributes().Any(attr => attr.AttributeClass?.ToDisplayString() == $"ZeroGames.ZSharp.Emit.Specifier.{FieldSpecifierName}Attribute") ?? false)
			{
				if (!_symbolMap.TryGetValue(typeSymbol, out var implicitBase))
				{
					implicitBase = true;
				}

				if (implicitBase)
				{
					implicitBase &= classDeclarationSyntax.BaseList?.Types.All(t => context.SemanticModel.GetTypeInfo(t.Type).Type!.TypeKind == TypeKind.Interface) ?? true;
				}

				_symbolMap[typeSymbol] = implicitBase;
			}
		}
	}
	
	public IReadOnlyDictionary<ITypeSymbol, bool> SymbolMap => _symbolMap;

	protected abstract string FieldSpecifierName { get; }
	
	private readonly Dictionary<ITypeSymbol, bool> _symbolMap = [];
	
}


