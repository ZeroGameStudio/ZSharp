﻿// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public class ExportedDelegateBuilder(string namespaceName, string typeName, string? unrealFieldPath, EDelegateKind kind, string conjugateKey) : GeneratedCompositeTypeBuilderBase<ClassDefinition>(namespaceName, typeName, unrealFieldPath)
{
	
	public EDelegateKind Kind { get; } = kind;
	public TypeReference? ReturnType { get; set; }
	public ParameterDeclaration[]? Parameters { get; set; }
	public string? OuterClassName { get; set; }
	public string ConjugateKey { get; set; } = conjugateKey;

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
		
		AddAttributeAfter("ConjugateKey", $"\"{ConjugateKey}\"");
		
		definition.AddMember(new Block($"public partial UnrealObject {BindMethodName}(Signature @delegate) => base.{BindMethodName}(@delegate);"));
		definition.AddMember(new Block($"public partial UnrealObject {BindMethodName}<TState>(Signature<TState> @delegate, TState state) => base.{BindMethodName}(@delegate, state);"));
		
		definition.AddMember(new MethodDefinition(EMemberVisibility.Public, ExecuteMethodName, ReturnType, Parameters) { Modifiers = EMemberModifiers.Partial, Body = new StrangeZCallBodyBuilder($"base.{ExecuteMethodName}", ReturnType, false, Parameters).Build() });
	}

	protected override void PreAddMainType(CompilationUnit compilationUnit, ClassDefinition? outerClassDefinition)
	{
		base.PreAddMainType(compilationUnit, outerClassDefinition);

		ClassDefinition abstractionDefinition = new(false, EMemberVisibility.Public, TypeName);
		abstractionDefinition.Modifiers |= EMemberModifiers.Sealed | EMemberModifiers.Partial;
		abstractionDefinition.AddBaseType(BaseType);

		TypeReference? signatureReturnType = ReturnType is not null ? ToSignatureParameterDecl(new(EParameterKind.Out, ReturnType.Value, string.Empty)).Type : null;
		ParameterDeclaration[]? signatureParameters = Parameters?.Select(ToSignatureParameterDecl).ToArray();

		MethodDefinition statelessSignature = new(EMemberVisibility.Public, "Signature", signatureReturnType, signatureParameters)
		{
			IsDelegate = true,
		};
		abstractionDefinition.AddMember(statelessSignature);

		string stateParameterName = signatureParameters is not null && signatureParameters.Any(p => p.Name is "state") ? "userState" : "state";
		ParameterDeclaration[] statefulSignatureParameters = [..signatureParameters ?? [], new(EParameterKind.In, new("TState", null), stateParameterName)];
		
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

		return " => BuildConjugate_Black(StaticSignature.Unmanaged);";
	}

	protected override bool HasBlackConstructor => Kind != EDelegateKind.Sparse;
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
	
	private string BindMethodName => Kind == EDelegateKind.Unicast ? "Bind" : "Add";
	private string ExecuteMethodName => Kind == EDelegateKind.Unicast ? "Execute" : "Broadcast";

	private string BaseType => Kind switch
	{
		EDelegateKind.Unicast => "UnrealDelegateBase",
		EDelegateKind.Multicast => "UnrealMulticastInlineDelegateBase",
		EDelegateKind.Sparse => "UnrealMulticastSparseDelegateBase",
		_ => throw new NotSupportedException()
	};

}


