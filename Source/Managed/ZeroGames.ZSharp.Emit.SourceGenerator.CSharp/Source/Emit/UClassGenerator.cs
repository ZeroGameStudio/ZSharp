// Copyright Zero Games. All Rights Reserved.

#pragma warning disable RS1035 // Do not use banned APIs for analyzers

using System.Collections.Immutable;
using System.Text;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.Text;
using ZeroGames.ZSharp.CodeDom.CSharp;

namespace ZeroGames.ZSharp.Emit.SourceGenerator.CSharp;

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
		
		foreach (var pair in receiver.SymbolMap)
		{
			GenerateUClass(pair.Key, context, pair.Value);
		}
	}
	
	private class UClassSyntaxReceiver : UFieldSyntaxReceiverBase
	{
		protected override string FieldSpecifierName => "UClass";
	}

	private void GenerateUClass(ITypeSymbol uclassSymbol, GeneratorExecutionContext context, bool implicitBase)
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
		string namespaceName = EmitGeneratorHelper.GetTypeNamespace(uclassSymbol);

		EmittedClassBuilder builder = new(namespaceName, className, implicitBase);
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
				usings.Add(EmitGeneratorHelper.GetTypeNamespace(method.ReturnType));
			}
			
			List<ParameterDeclaration> parameters = new();
			foreach (var parameter in method.Parameters)
			{
				usings.Add(EmitGeneratorHelper.GetTypeNamespace(parameter.Type));
				
				parameters.Add(new(EmitGeneratorHelper.RefKindToParameterKind(parameter.RefKind), EmitGeneratorHelper.GetTypeReference(parameter.Type), parameter.Name));
			}
			
			ImmutableArray<AttributeData> attributes = method.GetAttributes();
			builder.AddEventMethod
			(
				EmitGeneratorHelper.AccessibilityToMemberVisibility(method.DeclaredAccessibility),
				method.Name,
				!attributes.Any(attr => SymbolEqualityComparer.Default.Equals(attr.AttributeClass, blueprintImplementableEventSpecifierSymbol)),
				attributes.Any(attr => SymbolEqualityComparer.Default.Equals(attr.AttributeClass, withValidationSpecifierSymbol)),
				attributes.Any(attr => SymbolEqualityComparer.Default.Equals(attr.AttributeClass, sealedEventSpecifierSymbol)),
				!method.ReturnsVoid ? EmitGeneratorHelper.GetTypeReference(method.ReturnType) : null,
				parameters.ToArray()
			);
		}
		
		var properties = uclassSymbol.GetMembers()
			.OfType<IPropertySymbol>()
			.Where(property => property.GetAttributes().Any(attr => SymbolEqualityComparer.Default.Equals(attr.AttributeClass, upropertySymbol)));

		foreach (var property in properties)
		{
			usings.Add(EmitGeneratorHelper.GetTypeNamespace(property.Type));
			
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
				EmitGeneratorHelper.AccessibilityToMemberVisibility(property.DeclaredAccessibility),
				EmitGeneratorHelper.GetTypeReference(property.Type),
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

}


