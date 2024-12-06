// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public enum EExportedDelegateKind
{
	Unicast,
	Multicast,
	Sparse,
}

public class ExportedDelegateBuilder(string namespaceName, string typeName, string? unrealFieldPath, EExportedDelegateKind kind) : GeneratedCompositeTypeBuilderBase<ClassDefinition>(namespaceName, typeName, unrealFieldPath)
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

	protected override ClassDefinition AllocateTypeDefinition() => new(false, EMemberVisibility.Public, TypeName) { Modifiers = EMemberModifiers.Unsafe };

	protected override void BuildTypeDefinition(ClassDefinition definition)
	{
		base.BuildTypeDefinition(definition);
		
		definition.AddMember(new Block($"public partial UnrealObject {BindMethodName}(Signature @delegate) => {InteropClassPrefix}_Interop.{BindMethodName}ManagedDelegate(ConjugateHandle.FromConjugate(this), GCHandle.Alloc(@delegate)).GetTargetChecked<UnrealObject>();"));
		definition.AddMember(new MethodDefinition(EMemberVisibility.Public, ExecuteMethodName, ReturnType, Parameters) { Modifiers = EMemberModifiers.Partial, Body = new StrangeZCallBodyBuilder($"{InteropClassPrefix}_Interop.{ExecuteMethodName}", ReturnType, false, Parameters).Build() });
	}

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

		abstractionDefinition.AddMember(new Block($"public partial UnrealObject {BindMethodName}(Signature @delegate);"));
		abstractionDefinition.AddMember(new MethodDefinition(EMemberVisibility.Public, ExecuteMethodName, ReturnType, Parameters) { Modifiers = EMemberModifiers.Partial });
		
		if (outerClassDefinition is null)
		{
			compilationUnit.AddType(abstractionDefinition);
		}
		else
		{
			outerClassDefinition.AddMember(abstractionDefinition);
		}
	}

	protected override string GetBlackConstructorBody()
	{
		if (!HasBlackConstructor)
		{
			return base.GetBlackConstructorBody();
		}

		return " => Unmanaged = MasterAlcCache.Instance.BuildConjugate(this, StaticSignature.Unmanaged);";
	}

	protected override bool HasBlackConstructor => Kind != EExportedDelegateKind.Sparse;
	protected override string RedConstructorVisibility => "private";

	protected override string StaticFieldInterfaceName => "IStaticSignature";
	protected override string StaticFieldTypeName => "DelegateFunction";
	protected override string StaticFieldPropertyName => "StaticSignature";
	
	private string BindMethodName => Kind == EExportedDelegateKind.Unicast ? "Bind" : "Add";
	private string ExecuteMethodName => Kind == EExportedDelegateKind.Unicast ? "Execute" : "Broadcast";

	private string InteropClassPrefix => Kind switch
	{
		EExportedDelegateKind.Unicast => "UnrealDelegate",
		EExportedDelegateKind.Multicast => "UnrealMulticastInlineDelegate",
		EExportedDelegateKind.Sparse => "UnrealMulticastSparseDelegate",
		_ => throw new NotSupportedException()
	};

}


