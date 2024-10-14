// Copyright Zero Games. All Rights Reserved.

using System.Collections.Immutable;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.CSharp.Syntax;
using Microsoft.CodeAnalysis.Diagnostics;

namespace UClassAnalyzer;

[DiagnosticAnalyzer(LanguageNames.CSharp)]
public class CommonAnalyzer : DiagnosticAnalyzer
{
    public const string DIAGNOSTIC_ID = "ZS0001";
    private static readonly LocalizableString Title = "UClass class must be partial and not abstract or static";
    private static readonly LocalizableString MessageFormat = "Class '{0}' marked with [UClass] must be partial and cannot be abstract or static";
    private static readonly LocalizableString Description = "Classes marked with [UClass] must follow specific rules: they must be partial and cannot be abstract or static.";
    private const string Category = "Usage";

    private static DiagnosticDescriptor Rule = new(DIAGNOSTIC_ID, Title, MessageFormat, Category, DiagnosticSeverity.Error, isEnabledByDefault: true, description: Description);

    public override ImmutableArray<DiagnosticDescriptor> SupportedDiagnostics => ImmutableArray.Create(Rule);

    public override void Initialize(AnalysisContext context)
    {
        context.ConfigureGeneratedCodeAnalysis(GeneratedCodeAnalysisFlags.None);
        context.EnableConcurrentExecution();
        context.RegisterSyntaxNodeAction(AnalyzeClassDeclaration, SyntaxKind.ClassDeclaration);
    }

    private static void AnalyzeClassDeclaration(SyntaxNodeAnalysisContext context)
    {
        var classDeclaration = (ClassDeclarationSyntax)context.Node;

        var semanticModel = context.SemanticModel;
        
        var hasUClassAttribute = classDeclaration.AttributeLists
            .SelectMany(a => a.Attributes)
            .Any(a => a.Name.ToString() == "UClass" || a.Name.ToString() == "UClassAttribute");

        if (!hasUClassAttribute)
            return;

        var isPartial = classDeclaration.Modifiers.Any(SyntaxKind.PartialKeyword);
        if (!isPartial)
        {
            var diagnostic = Diagnostic.Create(Rule, classDeclaration.Identifier.GetLocation(), classDeclaration.Identifier.Text);
            context.ReportDiagnostic(diagnostic);
        }

        var isAbstractOrStatic = classDeclaration.Modifiers.Any(SyntaxKind.AbstractKeyword) ||
                                 classDeclaration.Modifiers.Any(SyntaxKind.StaticKeyword);
        if (isAbstractOrStatic)
        {
            var diagnostic = Diagnostic.Create(Rule, classDeclaration.Identifier.GetLocation(), classDeclaration.Identifier.Text);
            context.ReportDiagnostic(diagnostic);
        }
    }
}


