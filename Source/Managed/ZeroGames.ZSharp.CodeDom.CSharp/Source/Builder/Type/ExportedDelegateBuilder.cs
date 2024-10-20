﻿// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public enum EExportedDelegateKind
{
	Unicast,
	Multicast,
	Sparse,
}

public class ExportedDelegateBuilder(string namespaceName, string typeName, string? unrealFieldPath, EExportedDelegateKind kind) : ExportedCompositeTypeBuilderBase<ClassDefinition>(namespaceName, typeName, unrealFieldPath)
{
	
	public EExportedDelegateKind Kind { get; } = kind;
	public TypeReference? ReturnType { get; set; }
	public ParameterDeclaration[]? Parameters { get; set; }
	public string? OuterClassName { get; set; }

	protected override ClassDefinition? GetOuterClassDefinition()
	{
		if (OuterClassName is not null)
		{
			return new(false, EMemberVisibility.Default, OuterClassName)
			{
				Modifiers = EMemberModifiers.Partial,
			};
		}
		
		return base.GetOuterClassDefinition();
	}

	protected override ClassDefinition AllocateTypeDefinition() => new(false, EMemberVisibility.Default, TypeName);

	protected override void PreAddMainType(CompilationUnit compilationUnit, ClassDefinition? outerClassDefinition)
	{
		base.PreAddMainType(compilationUnit, outerClassDefinition);

		ClassDefinition abstractionDefinition = new(false, EMemberVisibility.Public, TypeName);
		abstractionDefinition.Modifiers |= EMemberModifiers.Sealed | EMemberModifiers.Partial;

		switch (Kind)
		{
			case EExportedDelegateKind.Unicast:
			{
				abstractionDefinition.AddBaseType("UnrealDelegateBase");
				break;
			}
			case EExportedDelegateKind.Multicast:
			{
				abstractionDefinition.AddBaseType("UnrealMulticastInlineDelegateBase");
				abstractionDefinition.AddAttributeListAfter(new AttributeDeclaration("Multicast"));
				break;
			}
			case EExportedDelegateKind.Sparse:
			{
				abstractionDefinition.AddBaseType("UnrealMulticastSparseDelegateBase");
				abstractionDefinition.AddAttributeListAfter(new AttributeDeclaration("Sparse"));
				break;
			}
			default:
			{
				throw new NotSupportedException();
			}
		}

		MethodDefinition delegateDecl = new(EMemberVisibility.Public, "Signature", ReturnType, Parameters);
		delegateDecl.IsDelegate = true;
		abstractionDefinition.AddMember(delegateDecl);

		string bindMethodName = Kind == EExportedDelegateKind.Unicast ? "Bind" : "Add";
		abstractionDefinition.AddMember(new Block($"public UnrealObject {bindMethodName}(Signature @delegate) => base.{bindMethodName}(@delegate);"));

		if (outerClassDefinition is null)
		{
			compilationUnit.AddType(abstractionDefinition);
		}
		else
		{
			outerClassDefinition.AddMember(abstractionDefinition);
		}
	}

	protected override IReadOnlyList<string> GetBaseConstructorExtraArguments() => [ "typeof(Signature)" ];

	protected override bool BlackConstructorNeedsCallBase => true;

	protected override string StaticFieldInterfaceName => "IStaticSignature";
	protected override string StaticFieldTypeName => "DelegateFunction";
	protected override string StaticFieldPropertyName => "StaticSignature";
}


