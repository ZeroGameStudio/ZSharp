// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public abstract class ExportedCompositeTypeBuilderBase<TDefinition>(string namespaceName, string typeName) : ExportedTypeBuilderBase<TDefinition>(namespaceName, typeName) where TDefinition : CompositeTypeDefinitionBase
{
	
	protected override void BuildTypeDefinition(TDefinition definition)
	{
		definition.Modifiers |= EMemberModifiers.Partial;

		if (GenerateIntrinsicContent)
		{
			definition.AddBaseType($"IConjugate<{TypeName}>");
			definition.AddBaseType("IStaticUnrealFieldPath");
			definition.AddBaseType(StaticFieldInterfaceName);
			
			IReadOnlyList<string> baseConstructorExtraArguments = GetBaseConstructorExtraArguments();
			string blackConstructorArguments = HasBlackConstructor ? string.Join(", ", baseConstructorExtraArguments) : string.Empty;
			string redConstructorArguments = string.Join(", ", baseConstructorExtraArguments.Append("unmanaged"));

			string blackConstructor = HasBlackConstructor ? $"public {TypeName}() : base({blackConstructorArguments}){{}}{Environment.NewLine}" : string.Empty;
		
			string header =
$@"public static {TypeName} BuildConjugate(IntPtr unmanaged) => new(unmanaged);
public new static string StaticUnrealFieldPath => __UNREAL_FIELD_PATH;
public new static {StaticFieldTypeName} {StaticFieldPropertyName} => UnrealObjectGlobals.LowLevelFindObject<{StaticFieldTypeName}>(__UNREAL_FIELD_PATH)!;
public override string UnrealFieldPath => __UNREAL_FIELD_PATH;
{blackConstructor}public {TypeName}(IntPtr unmanaged) : base({redConstructorArguments}){{}}
private const string __UNREAL_FIELD_PATH = {UnrealFieldPathLiteralText};";
			Block headerBlock = new(header);
		
			definition.AddMember(headerBlock);
		}
	}

	protected virtual IReadOnlyList<string> GetBaseConstructorExtraArguments() => [];

	protected virtual bool HasBlackConstructor => true;
	protected abstract string StaticFieldInterfaceName { get; }
	protected abstract string StaticFieldTypeName { get; }
	protected abstract string StaticFieldPropertyName { get; }

}


