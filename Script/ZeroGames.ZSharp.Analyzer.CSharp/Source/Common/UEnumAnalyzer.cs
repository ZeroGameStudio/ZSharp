// Copyright Zero Games. All Rights Reserved.

using System.Collections.Immutable;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.CSharp.Syntax;
using Microsoft.CodeAnalysis.Diagnostics;

namespace ZeroGames.ZSharp.Analyzer.CSharp;

[DiagnosticAnalyzer(LanguageNames.CSharp)]
public class UEnumAnalyzer : DiagnosticAnalyzer
{
    
    public const string DIAGNOSTIC_ID = "ZS0002";

    public override ImmutableArray<DiagnosticDescriptor> SupportedDiagnostics => [ _rule ];

    public override void Initialize(AnalysisContext context)
    {
        context.ConfigureGeneratedCodeAnalysis(GeneratedCodeAnalysisFlags.None);
        context.EnableConcurrentExecution();
        context.RegisterSyntaxNodeAction(AnalyzeClassDeclaration, SyntaxKind.EnumDeclaration);
    }

    private static void AnalyzeClassDeclaration(SyntaxNodeAnalysisContext context)
    {
        var enumDeclaration = (EnumDeclarationSyntax)context.Node;
        
        var hasUEnumAttribute = enumDeclaration.AttributeLists
            .SelectMany(a => a.Attributes)
            .Any(a => a.Name.ToString() is "UEnum" or "UEnumAttribute");

        if (!hasUEnumAttribute)
        {
            return;
        }

        var unrealFieldPathAttribute = enumDeclaration.AttributeLists
            .SelectMany(a => a.Attributes)
            .SingleOrDefault(a => a.Name.ToString() is "UnrealFieldPath" or "UnrealFieldPathAttribute");

        var arguments = unrealFieldPathAttribute?.ArgumentList?.Arguments ?? default;
        if (arguments.Count != 1)
        {
            goto error;
        }

        var semanticModel = context.SemanticModel;
        var enumSymbol = semanticModel.GetDeclaredSymbol(enumDeclaration)!;
        var @namespace = enumSymbol.ContainingNamespace;
        if (@namespace is null)
        {
            goto error;
        }
                
        var expectedPath = $"\"/Script/{@namespace.Name}.{enumSymbol.Name}\"";
        var argument = arguments[0];
        if (argument.Expression is LiteralExpressionSyntax literalExpr &&
            literalExpr.IsKind(SyntaxKind.StringLiteralExpression) &&
            literalExpr.Token.Text == expectedPath)
        {
            return;
        }

        error:
        var diagnostic = Diagnostic.Create(_rule, enumDeclaration.Identifier.GetLocation(), enumDeclaration.Identifier.Text);
        context.ReportDiagnostic(diagnostic);
    }
    
    private const string CATEGORY = "Usage";
    private static readonly LocalizableString _title = "UEnum";
    private static readonly LocalizableString _messageFormat = "Enum '{0}' marked with [UEnum] must have [UnrealFieldPath] with path \"/Script/<ModuleName>.<EnumName>\" attribute on it";
    private static readonly LocalizableString _description = "Enums marked with [UEnum] must follow specific rules: they must have [UnrealFieldPath] with path \"/Script/<ModuleName>.<EnumName>\" attribute on them.";

    private static readonly DiagnosticDescriptor _rule = new(DIAGNOSTIC_ID, _title, _messageFormat, CATEGORY, DiagnosticSeverity.Error, isEnabledByDefault: true, description: _description);
    
}


