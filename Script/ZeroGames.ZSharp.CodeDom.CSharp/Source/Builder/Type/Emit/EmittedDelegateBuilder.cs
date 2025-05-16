// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public class EmittedDelegateBuilder(string namespaceName, string typeName, string? outerClassName, EDelegateKind kind) : GeneratedCompositeTypeBuilderBase<ClassDefinition>(namespaceName, typeName, StaticGetUnrealFieldPath(namespaceName, typeName, outerClassName))
{
	
	public EDelegateKind Kind { get; } = kind;
	public TypeReference? ReturnType { get; set; }
	public ParameterDeclaration[]? Parameters { get; set; }
	public string? OuterClassName { get; } = outerClassName;

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

	protected override ClassDefinition AllocateTypeDefinition() => new(false, EMemberVisibility.Default, TypeName) { Modifiers = EMemberModifiers.Unsafe };

	protected override void BuildTypeDefinition(ClassDefinition definition)
	{
		base.BuildTypeDefinition(definition);
		
		AddAttributeAfter("ConjugateKey", UNREAL_FIELD_PATH_CONST);
		
		definition.Modifiers |= EMemberModifiers.Sealed | EMemberModifiers.Partial;

		TypeReference? signatureReturnType = ReturnType is not null ? DelegateHelper.ToSignatureParameterDecl(new(EParameterKind.Out, ReturnType.Value, string.Empty)).Type : null;
		ParameterDeclaration[]? signatureParameters = Parameters?.Select(DelegateHelper.ToSignatureParameterDecl).ToArray();

		string stateParameterName = signatureParameters is not null && signatureParameters.Any(p => p.Name is "state") ? "userState" : "state";
		ParameterDeclaration[] statefulSignatureParameters = [..signatureParameters ?? [], new(EParameterKind.In, new("TState", null), stateParameterName)];
		
		MethodDefinition statefulSignature = new(EMemberVisibility.Public, "Signature<in TState>", signatureReturnType, statefulSignatureParameters)
		{
			IsDelegate = true,
		};
		definition.AddMember(statefulSignature);

		definition.AddMember(new Block($"public UObject {BindMethodName}(Signature @delegate) => base.{BindMethodName}(@delegate);"));
		definition.AddMember(new Block($"public UObject {BindMethodName}<TState>(Signature<TState> @delegate, TState state) => base.{BindMethodName}(@delegate, state);"));
		
		definition.AddMember(new MethodDefinition(EMemberVisibility.Public, ExecuteMethodName, ReturnType, Parameters) { Body = new StrangeZCallBodyBuilder($"base.{ExecuteMethodName}", ReturnType, false, Parameters).Build() });
	}

	protected override string GetBlackConstructorBody()
	{
		if (!HasBlackConstructor)
		{
			return base.GetBlackConstructorBody();
		}

		return " => BuildConjugate_Black(StaticSignature.Unmanaged);";
	}

	protected override bool HasBlackConstructor => Kind != EDelegateKind.Sparse;
	protected override string RedConstructorVisibility => "private";

	protected override string StaticFieldInterfaceName => "IStaticSignature";
	protected override string StaticFieldTypeName => "UDelegateFunction";
	protected override string StaticFieldPropertyName => "StaticSignature";

	private static string StaticGetUnrealFieldPath(string namespaceName, string typeName, string? outerClassName)
	{
		string typeSegment = outerClassName is null ? typeName : $"{outerClassName}:{typeName}";
		return $"/Script/{namespaceName.Split('.').Last()}.{typeSegment.Substring(1)}__DelegateSignature";
	}
	
	private string BindMethodName => Kind == EDelegateKind.Unicast ? "Bind" : "Add";
	private string ExecuteMethodName => Kind == EDelegateKind.Unicast ? "Execute" : "Broadcast";

}


