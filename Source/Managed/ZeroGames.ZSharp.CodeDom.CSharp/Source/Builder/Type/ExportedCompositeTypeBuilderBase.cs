// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public abstract class ExportedCompositeTypeBuilderBase<TDefinition>(string namespaceName, string typeName, string? unrealFieldPath) : ExportedTypeBuilderBase<TDefinition>(namespaceName, typeName, unrealFieldPath) where TDefinition : CompositeTypeDefinitionBase
{
	
	protected override void BuildTypeDefinition(TDefinition definition)
	{
		definition.Modifiers |= EMemberModifiers.Partial;

		if (GenerateIntrinsicContent)
		{
			if (HasBuildConjugate)
			{
				definition.AddBaseType($"IConjugate<{TypeName}>");
			}

			bool isConcreteUnrealField = HasUnrealFieldPath && definition.Kind != ETypeKind.Interface;
			if (isConcreteUnrealField)
			{
				definition.AddBaseType("IStaticUnrealFieldPath");
				definition.AddBaseType(StaticFieldInterfaceName);
			}
			
			string buildConjugate = HasBuildConjugate ? $"public new static {TypeName} BuildConjugate(IntPtr unmanaged) => new(unmanaged);" : string.Empty;
			// Interfaces don't inherit static field interfaces but still have these static members (@see IUnrealObject.cs).
			string staticUnrealFieldPath = HasUnrealFieldPath ? $"public new static string StaticUnrealFieldPath => {UNREAL_FIELD_PATH_CONST};" : string.Empty;
			string staticField = HasUnrealFieldPath ? $"public new static {StaticFieldTypeName} {StaticFieldPropertyName} => UnrealObjectGlobals.LowLevelFindObject<{StaticFieldTypeName}>({UNREAL_FIELD_PATH_CONST})!;" : string.Empty;
			string overrideUnrealFieldPath = isConcreteUnrealField ? $"public override string UnrealFieldPath => {UNREAL_FIELD_PATH_CONST};" : string.Empty;
			
			IReadOnlyList<string> baseConstructorExtraArguments = GetBaseConstructorExtraArguments();
			string blackConstructorArguments = HasBlackConstructor ? string.Join(", ", baseConstructorExtraArguments) : string.Empty;
			string blackConstructorCallBase = BlackConstructorNeedsCallBase ? $" : base({blackConstructorArguments})" : string.Empty;
			string blackConstructor = HasBlackConstructor ? $"public {TypeName}(){blackConstructorCallBase}{GetBlackConstructorBody()}" : string.Empty;
			
			string redConstructorArguments = string.Join(", ", baseConstructorExtraArguments.Append("unmanaged"));
			string redConstructor = HasRedConstructor ? $"public {TypeName}(IntPtr unmanaged) : base({redConstructorArguments}){{}}" : string.Empty;
			
			string unrealFieldPathConst = HasUnrealFieldPathConst ? $"private const string {UNREAL_FIELD_PATH_CONST} = {UnrealFieldPathLiteralText};" : string.Empty;

			string[] headers = [ buildConjugate, staticUnrealFieldPath, staticField, overrideUnrealFieldPath, blackConstructor, redConstructor, unrealFieldPathConst ];
			string header = string.Join(Environment.NewLine, headers.Where(h => !string.IsNullOrWhiteSpace(h)));
			Block headerBlock = new(header);
		
			definition.AddMember(headerBlock);
		}
	}

	protected virtual IReadOnlyList<string> GetBaseConstructorExtraArguments() => [];
	protected virtual string GetBlackConstructorBody() => "{}";

	protected virtual bool HasBuildConjugate => true;
	
	protected virtual bool HasBlackConstructor => true;
	protected virtual bool BlackConstructorNeedsCallBase => false;

	protected virtual bool HasRedConstructor => true;
	
	protected abstract string StaticFieldInterfaceName { get; }
	protected abstract string StaticFieldTypeName { get; }
	protected abstract string StaticFieldPropertyName { get; }

}


