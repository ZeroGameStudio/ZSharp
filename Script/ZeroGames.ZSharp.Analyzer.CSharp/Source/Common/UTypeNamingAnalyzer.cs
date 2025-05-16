// Copyright Zero Games. All Rights Reserved.

using System.Collections.Immutable;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.CSharp.Syntax;
using Microsoft.CodeAnalysis.Diagnostics;

namespace ZeroGames.ZSharp.Analyzer.CSharp;

[DiagnosticAnalyzer(LanguageNames.CSharp)]
public class UTypeNamingAnalyzer : DiagnosticAnalyzer
{
    
    public const string DIAGNOSTIC_ID = "ZS0001";

    public override ImmutableArray<DiagnosticDescriptor> SupportedDiagnostics => [ _rule ];

    public override void Initialize(AnalysisContext context)
    {
        context.ConfigureGeneratedCodeAnalysis(GeneratedCodeAnalysisFlags.None);
        context.EnableConcurrentExecution();
        context.RegisterSyntaxNodeAction(AnalyzeClassDeclaration, SyntaxKind.ClassDeclaration, SyntaxKind.InterfaceDeclaration, SyntaxKind.EnumDeclaration);
    }

    private static void AnalyzeClassDeclaration(SyntaxNodeAnalysisContext context)
    {
        INamedTypeSymbol actorTypeSymbol = context.Compilation.GetTypeByMetadataName("ZeroGames.ZSharp.UnrealEngine.Engine.AActor")!;

        SemanticModel semanticModel = context.SemanticModel;
        var typeDecl = (BaseTypeDeclarationSyntax)context.Node;
        if (semanticModel.GetDeclaredSymbol(typeDecl) is not { } typeSymbol)
        {
            return;
        }
        bool isActor = false;
        INamedTypeSymbol? current = typeSymbol;
        while (current is not null)
        {
            if (SymbolEqualityComparer.Default.Equals(current, actorTypeSymbol))
            {
                isActor = true;
                break;
            }
            
            current = current.BaseType;
        }

        foreach (var attribute in typeDecl.AttributeLists.SelectMany(a => a.Attributes))
        {
            bool found = false;
            string name = attribute.Name.ToString();
            if (name is "UClass" or "UClassAttribute")
            {
                found = true;
                
                if (isActor)
                {
                    if (!RequiresPrefix(typeSymbol, 'A'))
                    {
                        goto error;
                    }
                }
                else
                {
                    if (!RequiresPrefix(typeSymbol, 'U'))
                    {
                        goto error;
                    }
                }
            }
            else if (name is "UStruct" or "UStructAttribute" or "UDelegate" or "UDelegateAttribute")
            {
                found = true;
                if (!RequiresPrefix(typeSymbol, 'F'))
                {
                    goto error;
                }
            }
            else if (name is "UInterface" or "UInterfaceAttribute")
            {
                found = true;
                if (!RequiresPrefix(typeSymbol, 'I'))
                {
                    goto error;
                }
            }
            else if (name is "UEnum" or "UEnumAttribute")
            {
                found = true;
                if (!RequiresPrefix(typeSymbol, 'E'))
                {
                    goto error;
                }
            }

            if (found)
            {
                break;
            }
        }

        return;
        
        error:
        SyntaxToken identifier = ((BaseTypeDeclarationSyntax)context.Node).Identifier;
        var diagnostic = Diagnostic.Create(_rule, identifier.GetLocation(), identifier.Text);
        context.ReportDiagnostic(diagnostic);
    }

    private static bool RequiresPrefix(INamedTypeSymbol typeSymbol, char prefix)
        => typeSymbol.Name.Length > 1 && typeSymbol.Name.StartsWith(prefix.ToString()) && char.IsUpper(typeSymbol.Name[1]);
    
    private const string CATEGORY = "Usage";
    private static readonly LocalizableString _title = "Unreal types must start with correct prefix (UAFIE) and an upper letter";
    private static readonly LocalizableString _messageFormat = "Unreal type '{0}' must start with correct prefix (UAFIE) and an upper letter";
    private static readonly LocalizableString _description = "Unreal type must start with: Non-Actor UObject - U, Actor - A, Struct/Delegate - F, Interface - I, Enum - E, and an upper letter.";

    private static readonly DiagnosticDescriptor _rule = new(DIAGNOSTIC_ID, _title, _messageFormat, CATEGORY, DiagnosticSeverity.Error, isEnabledByDefault: true, description: _description);
    
}