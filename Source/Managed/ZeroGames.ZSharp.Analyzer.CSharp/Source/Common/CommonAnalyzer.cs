// Copyright Zero Games. All Rights Reserved.

using System.Collections.Immutable;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.CSharp.Syntax;
using Microsoft.CodeAnalysis.Diagnostics;

namespace ZeroGames.ZSharp.Analyzer.CSharp;

[DiagnosticAnalyzer(LanguageNames.CSharp)]
public class CommonAnalyzer : DiagnosticAnalyzer
{
    
    public const string DIAGNOSTIC_ID = "ZS0001";

    public override ImmutableArray<DiagnosticDescriptor> SupportedDiagnostics => [ _rule ];

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
            var diagnostic = Diagnostic.Create(_rule, classDeclaration.Identifier.GetLocation(), classDeclaration.Identifier.Text);
            context.ReportDiagnostic(diagnostic);
        }

        var isAbstractOrStatic = classDeclaration.Modifiers.Any(SyntaxKind.AbstractKeyword) ||
                                 classDeclaration.Modifiers.Any(SyntaxKind.StaticKeyword);
        if (isAbstractOrStatic)
        {
            var diagnostic = Diagnostic.Create(_rule, classDeclaration.Identifier.GetLocation(), classDeclaration.Identifier.Text);
            context.ReportDiagnostic(diagnostic);
        }
    }
    
    private const string CATEGORY = "Usage";
    private static readonly LocalizableString _title = "UClass class must be partial and not abstract or static";
    private static readonly LocalizableString _messageFormat = "Class '{0}' marked with [UClass] must be partial and cannot be abstract or static";
    private static readonly LocalizableString _description = "Classes marked with [UClass] must follow specific rules: they must be partial and cannot be abstract or static.";

    private static readonly DiagnosticDescriptor _rule = new(DIAGNOSTIC_ID, _title, _messageFormat, CATEGORY, DiagnosticSeverity.Error, isEnabledByDefault: true, description: _description);
    
}


