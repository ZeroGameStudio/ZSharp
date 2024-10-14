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
    // Diagnostic ID 和描述信息
    public const string DIAGNOSTIC_ID = "UClassAnalyzer";
    private static readonly LocalizableString Title = "UClass class must be partial and not abstract or static";
    private static readonly LocalizableString MessageFormat = "Class '{0}' marked with [UClass] must be partial and cannot be abstract or static";
    private static readonly LocalizableString Description = "Classes marked with [UClass] must follow specific rules: they must be partial and cannot be abstract or static.";
    private const string Category = "Usage";

    // 定义 Diagnostic 的严重性和规则
    private static DiagnosticDescriptor Rule = new DiagnosticDescriptor(DIAGNOSTIC_ID, Title, MessageFormat, Category, DiagnosticSeverity.Error, isEnabledByDefault: true, description: Description);

    public override ImmutableArray<DiagnosticDescriptor> SupportedDiagnostics => ImmutableArray.Create(Rule);

    public override void Initialize(AnalysisContext context)
    {
        // 注册语法节点操作，检查类声明
        context.ConfigureGeneratedCodeAnalysis(GeneratedCodeAnalysisFlags.None);
        context.EnableConcurrentExecution();
        context.RegisterSyntaxNodeAction(AnalyzeClassDeclaration, SyntaxKind.ClassDeclaration);
    }

    private static void AnalyzeClassDeclaration(SyntaxNodeAnalysisContext context)
    {
        var classDeclaration = (ClassDeclarationSyntax)context.Node;

        var semanticModel = context.SemanticModel;
        
        // 查找 [UClass] 特性
        var hasUClassAttribute = classDeclaration.AttributeLists
            .SelectMany(a => a.Attributes)
            .Any(a => a.Name.ToString() == "UClass" || a.Name.ToString() == "UClassAttribute");

        if (!hasUClassAttribute)
            return;

        // 检查类是否为 partial
        var isPartial = classDeclaration.Modifiers.Any(SyntaxKind.PartialKeyword);
        if (!isPartial)
        {
            // 生成 Diagnostic
            var diagnostic = Diagnostic.Create(Rule, classDeclaration.Identifier.GetLocation(), classDeclaration.Identifier.Text);
            context.ReportDiagnostic(diagnostic);
        }

        // 检查类是否为 abstract 或 static
        var isAbstractOrStatic = classDeclaration.Modifiers.Any(SyntaxKind.AbstractKeyword) ||
                                 classDeclaration.Modifiers.Any(SyntaxKind.StaticKeyword);
        if (isAbstractOrStatic)
        {
            var diagnostic = Diagnostic.Create(Rule, classDeclaration.Identifier.GetLocation(), classDeclaration.Identifier.Text);
            context.ReportDiagnostic(diagnostic);
        }
    }
}


