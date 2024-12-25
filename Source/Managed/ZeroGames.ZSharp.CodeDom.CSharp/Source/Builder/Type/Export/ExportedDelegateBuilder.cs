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
		
		definition.AddMember(new Block($"public partial UnrealObject {BindMethodName}(Signature @delegate) => base.{BindMethodName}(@delegate);"));
		definition.AddMember(new Block($"public partial UnrealObject {BindMethodName}<TState>(Signature<TState> @delegate, TState state) => base.{BindMethodName}(@delegate, state);"));
		
		definition.AddMember(new MethodDefinition(EMemberVisibility.Public, ExecuteMethodName, ReturnType, Parameters) { Modifiers = EMemberModifiers.Partial, Body = new StrangeZCallBodyBuilder($"base.{ExecuteMethodName}", ReturnType, false, Parameters).Build() });
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

		TypeReference? signatureReturnType = ReturnType is not null ? ToSignatureParameterDecl(new(EParameterKind.Out, ReturnType.Value, string.Empty)).Type : null;
		ParameterDeclaration[]? signatureParameters = Parameters?.Select(ToSignatureParameterDecl).ToArray();

		MethodDefinition statelessSignature = new(EMemberVisibility.Public, "Signature", signatureReturnType, signatureParameters)
		{
			IsDelegate = true,
		};
		abstractionDefinition.AddMember(statelessSignature);

		ParameterDeclaration[] statefulSignatureParameters = [..signatureParameters ?? [], new(EParameterKind.In, new("TState", null), "state")];
		
		MethodDefinition statefulSignature = new(EMemberVisibility.Public, "Signature<TState>", signatureReturnType, statefulSignatureParameters)
		{
			IsDelegate = true,
		};
		abstractionDefinition.AddMember(statefulSignature);

		abstractionDefinition.AddMember(new Block($"public partial UnrealObject {BindMethodName}(Signature @delegate);"));
		abstractionDefinition.AddMember(new Block($"public partial UnrealObject {BindMethodName}<TState>(Signature<TState> @delegate, TState state);"));
		
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

	private ParameterDeclaration ToSignatureParameterDecl(ParameterDeclaration source)
	{
		EParameterKind kind = source.Kind;
		TypeReference sourceType = source.Type;
		string targetTypeName = sourceType.TypeName;
		if (kind == EParameterKind.In)
		{
			if (source.Type.IsNullInNotNullOut && targetTypeName.EndsWith("?"))
			{
				targetTypeName = targetTypeName.Substring(0, targetTypeName.Length - 1);
			}
		}
		else if (kind == EParameterKind.Out)
		{
			if (source.Type.IsNullInNotNullOut && !targetTypeName.EndsWith("?"))
			{
				targetTypeName += "?";
			}
		}

		AttributeDeclaration[]? targetAttributes = source.Attributes?.Declarations.ToArray();
		if (targetAttributes is not null)
		{
			for (int32 i = 0; i < targetAttributes.Length; ++i)
			{
				if (targetAttributes[i].Name == "NotNull")
				{
					targetAttributes[i] = new("AllowNull");
				}
			}
		}

		TypeReference targetType = new(targetTypeName, sourceType.UnderlyingType, sourceType.IsNullInNotNullOut);
		return new(kind, targetType, source.Name, source.DefaultValue, targetAttributes);
	}
	
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


