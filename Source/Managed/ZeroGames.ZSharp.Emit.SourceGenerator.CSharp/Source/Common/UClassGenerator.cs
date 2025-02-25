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

public class UClassSyntaxReceiver : ISyntaxContextReceiver
{
	
	public void OnVisitSyntaxNode(GeneratorSyntaxContext context)
	{
		if (context.Node is ClassDeclarationSyntax { AttributeLists.Count: > 0 } classDeclarationSyntax)
		{
			var typeSymbol = context.SemanticModel.GetDeclaredSymbol(classDeclarationSyntax) as ITypeSymbol;
			if (typeSymbol?.GetAttributes().Any(attr => attr.AttributeClass?.ToDisplayString() == "ZeroGames.ZSharp.Emit.Specifier.UClassAttribute") ?? false)
			{
				if (!_uclassSymbols.Contains(typeSymbol))
				{
					_uclassSymbols.Add(typeSymbol);
				}
			}
		}
	}
	
	public IReadOnlyList<ITypeSymbol> UClassSymbols => _uclassSymbols;

	private List<ITypeSymbol> _uclassSymbols = new();
	
}

[Generator]
public class UClassGenerator : ISourceGenerator
{

	public void Initialize(GeneratorInitializationContext context)
	{
		context.RegisterForSyntaxNotifications(() => new UClassSyntaxReceiver());
	}

	public void Execute(GeneratorExecutionContext context)
	{
		if (context.SyntaxContextReceiver is not UClassSyntaxReceiver receiver)
		{
			return;
		}
		
		foreach (var uclassSymbol in receiver.UClassSymbols)
		{
			GenerateUClass(uclassSymbol, context);
		}
	}

	private void GenerateUClass(ITypeSymbol uclassSymbol, GeneratorExecutionContext context)
	{
		INamedTypeSymbol ufunctionSpecifierSymbol = context.Compilation.GetTypeByMetadataName("ZeroGames.ZSharp.Emit.Specifier.UFunctionAttribute")!;
		INamedTypeSymbol eventBaseSpecifierSymbol = context.Compilation.GetTypeByMetadataName("ZeroGames.ZSharp.Emit.Specifier.EventSpecifierBase")!;
		INamedTypeSymbol blueprintImplementableEventSpecifierSymbol = context.Compilation.GetTypeByMetadataName("ZeroGames.ZSharp.Emit.Specifier.BlueprintImplementableEventAttribute")!;
		INamedTypeSymbol withValidationSpecifierSymbol = context.Compilation.GetTypeByMetadataName("ZeroGames.ZSharp.Emit.Specifier.WithValidationAttribute")!;
		INamedTypeSymbol sealedEventSpecifierSymbol = context.Compilation.GetTypeByMetadataName("ZeroGames.ZSharp.Emit.Specifier.SealedEventAttribute")!;
		INamedTypeSymbol upropertySymbol = context.Compilation.GetTypeByMetadataName("ZeroGames.ZSharp.Emit.Specifier.UPropertyAttribute")!;
		INamedTypeSymbol replicatedSpecifierSymbol = context.Compilation.GetTypeByMetadataName("ZeroGames.ZSharp.Emit.Specifier.ReplicatedAttribute")!;
		INamedTypeSymbol fieldNotifySpecifierSymbol = context.Compilation.GetTypeByMetadataName("ZeroGames.ZSharp.Emit.Specifier.FieldNotifyAttribute")!;
		
		string className = uclassSymbol.Name;
		string namespaceName = GetTypeNamespace(uclassSymbol);

		EmittedClassBuilder builder = new(namespaceName, className);
		List<string> usings = new();
		
		var methods = uclassSymbol.GetMembers()
			.OfType<IMethodSymbol>()
			.Where(method =>
			{
				ImmutableArray<AttributeData> attributes = method.GetAttributes();
				bool isUFunction = attributes.Any(attr => SymbolEqualityComparer.Default.Equals(attr.AttributeClass, ufunctionSpecifierSymbol));
				bool isNativeEvent = attributes.Any(attr =>
				{
					INamedTypeSymbol? currentClass = attr.AttributeClass;
					while (currentClass is not null)
					{
						if (SymbolEqualityComparer.Default.Equals(currentClass, eventBaseSpecifierSymbol))
						{
							return true;
						}

						currentClass = currentClass.BaseType;
					}

					return false;
				});

				return isUFunction && isNativeEvent;
			});
		
		foreach (var method in methods)
		{
			if (!method.ReturnsVoid)
			{
				usings.Add(GetTypeNamespace(method.ReturnType));
			}
			
			List<ParameterDeclaration> parameters = new();
			foreach (var parameter in method.Parameters)
			{
				usings.Add(GetTypeNamespace(parameter.Type));
				
				parameters.Add(new(RefKindToParameterKind(parameter.RefKind), GetTypeReference(parameter.Type), parameter.Name));
			}
			
			ImmutableArray<AttributeData> attributes = method.GetAttributes();
			builder.AddEventMethod
			(
				AccessibilityToMemberVisibility(method.DeclaredAccessibility),
				method.Name,
				!attributes.Any(attr => SymbolEqualityComparer.Default.Equals(attr.AttributeClass, blueprintImplementableEventSpecifierSymbol)),
				attributes.Any(attr => SymbolEqualityComparer.Default.Equals(attr.AttributeClass, withValidationSpecifierSymbol)),
				attributes.Any(attr => SymbolEqualityComparer.Default.Equals(attr.AttributeClass, sealedEventSpecifierSymbol)),
				!method.ReturnsVoid ? GetTypeReference(method.ReturnType) : null,
				parameters.ToArray()
			);
		}
		
		var properties = uclassSymbol.GetMembers()
			.OfType<IPropertySymbol>()
			.Where(property => property.GetAttributes().Any(attr => SymbolEqualityComparer.Default.Equals(attr.AttributeClass, upropertySymbol)));

		foreach (var property in properties)
		{
			usings.Add(GetTypeNamespace(property.Type));
			
			ImmutableArray<AttributeData> attributes = property.GetAttributes();
			AttributeData? fieldNotify = attributes.SingleOrDefault(attr => SymbolEqualityComparer.Default.Equals(attr.AttributeClass, fieldNotifySpecifierSymbol));
			List<string>? fieldNotifies = null;
			if (fieldNotify is not null)
			{
				fieldNotifies = [ $"nameof({property.Name})" ];
				var arg = fieldNotify.NamedArguments.SingleOrDefault(pair => pair.Key == "RelatedFields");
				if (!string.IsNullOrWhiteSpace(arg.Key))
				{
					foreach (var relatedField in arg.Value.Values)
					{
						fieldNotifies.Add($"\"{relatedField.Value}\"");
					}
				}
			}
			
			AttributeData? replicated = attributes.SingleOrDefault(attr => SymbolEqualityComparer.Default.Equals(attr.AttributeClass, replicatedSpecifierSymbol));
			bool needsMarkDirty = replicated is not null && !replicated.NamedArguments.Any(pair => pair.Key == "IsPushBased" && !(bool)pair.Value.Value!);
			if (needsMarkDirty)
			{
				usings.Add("ZeroGames.ZSharp.UnrealEngine.ZSharpRuntime");
			}
			
			builder.AddProperty
			(
				AccessibilityToMemberVisibility(property.DeclaredAccessibility),
				GetTypeReference(property.Type),
				property.Name,
				fieldNotifies?.ToArray(),
				needsMarkDirty
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

	private EParameterKind RefKindToParameterKind(RefKind refKind) => refKind switch
	{
		RefKind.None or RefKind.In => EParameterKind.In,
		RefKind.Out => EParameterKind.Out,
		RefKind.Ref or RefKind.RefReadOnlyParameter => EParameterKind.Ref,
		_ => throw new NotSupportedException()
	};

	private EMemberVisibility AccessibilityToMemberVisibility(Accessibility accessibility) => accessibility switch
	{
		Accessibility.Public => EMemberVisibility.Public,
		Accessibility.Protected => EMemberVisibility.Protected,
		_ => EMemberVisibility.Private
	};

	private string GetNormalizedTypeName(ITypeSymbol type)
	{
		string name = type.Name switch
		{
			nameof(Byte) => nameof(uint8),
			nameof(UInt16) => nameof(uint16),
			nameof(UInt32) => nameof(uint32),
			nameof(UInt64) => nameof(uint64),
			nameof(SByte) => nameof(int8),
			nameof(Int16) => nameof(int16),
			nameof(Int32) => nameof(int32),
			nameof(Int64) => nameof(int64),
			nameof(Single) => "float",
			nameof(Double) => "double",
			nameof(Boolean) => "bool",
			_ => string.Empty
		};

		if (name == string.Empty)
		{
			name = type.Name + (type.NullableAnnotation == NullableAnnotation.Annotated ? "?" : string.Empty);
			if (type is INamedTypeSymbol namedType && namedType.IsGenericType)
			{
				name += $"<{string.Join(", ", namedType.TypeArguments.Select(GetNormalizedTypeName))}>";
			}
		}

		return name;
	}

	private string GetTypeNamespace(ITypeSymbol type) => type.ContainingNamespace.ToString();

	private TypeReference GetTypeReference(ITypeSymbol type) => new(GetNormalizedTypeName(type), type.TypeKind == TypeKind.Enum ? "uint64" : null);
	
}


