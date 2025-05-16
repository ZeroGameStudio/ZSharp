// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public class EmittedScriptStructBuilder(string namespaceName, string typeName, bool implicitBase) : GeneratedCompositeTypeBuilderBase<ClassDefinition>(namespaceName, typeName, $"/Script/{namespaceName.Split('.').Last()}.{typeName.Substring(1)}")
{
	
	public EmittedPropertyDefinition AddProperty(EMemberVisibility visibility, TypeReference type, string name)
	{
		string zcallName = $"up:/{UnrealFieldPath}:{name}";
		PropertyDefinition property;
		{
			EMemberModifiers modifiers = EMemberModifiers.Partial;
			property = new ZCallPropertyBuilder(visibility, modifiers, name, zcallName, 0, type, false, false).Build(false);
			_properties.Add(property);
		}
		
		FieldDefinition zcallHandle = AddStaticField(new("ZCallHandle?", null), $"_zcallHandleFor{name}");

		return new(property, zcallHandle);
	}
	
	protected override ClassDefinition AllocateTypeDefinition() => new(false, EMemberVisibility.Default, TypeName);

	protected override void BuildTypeDefinition(ClassDefinition definition)
	{
		base.BuildTypeDefinition(definition);

		if (_implicitBase)
		{
			AddBaseTypeBefore("UnrealScriptStructBase");
		}
		
		AddBaseTypeAfter($"ICloneable<{TypeName}>");
		AddBaseTypeAfter($"IEquatable<{TypeName}>");
		AddBaseTypeAfter($"System.Numerics.IEqualityOperators<{TypeName}?, {TypeName}?, bool>");

		string systemInterfaceBlock =
$@"public {TypeName}({TypeName} other) : this() => Copy(other);
public new {TypeName} Clone() => new(this);
object ICloneable.Clone() => Clone();
public bool Equals({TypeName}? other) => ReferenceEquals(this, other) || other is not null && other.GetType() == typeof({TypeName}) && Identical(other);
public override bool Equals(object? obj) => obj is {TypeName} o && Equals(o);
public override int32 GetHashCode() => throw new NotSupportedException();
public static bool operator ==({TypeName}? left, {TypeName}? right) => Equals(left, right);
public static bool operator !=({TypeName}? left, {TypeName}? right) => !Equals(left, right);";
		definition.AddMember(new Block(systemInterfaceBlock));

		definition.Modifiers |= EMemberModifiers.Unsafe;
		
		AddAttributeAfter("ConjugateKey", UNREAL_FIELD_PATH_CONST);

		foreach (var property in _properties)
		{
			definition.AddMember(property);
		}

		foreach (var field in _fields)
		{
			definition.AddMember(field);
		}
	}

	protected override string GetBlackConstructorBody()
		=> $" {{ if (GetType() == typeof({TypeName})) BuildConjugate_Black(StaticStruct.Unmanaged); }}";

	protected override string StaticFieldInterfaceName => "IStaticStruct";
	protected override string StaticFieldTypeName => "UScriptStruct";
	protected override string StaticFieldPropertyName => "StaticStruct";
	
	private FieldDefinition AddStaticField(TypeReference type, string name)
	{
		FieldDefinition field = new(EMemberVisibility.Private, name, type)
		{
			Modifiers = EMemberModifiers.Static,
		};
		
		_fields.Add(field);

		return field;
	}
	
	private readonly List<PropertyDefinition> _properties = new();
	private readonly List<FieldDefinition> _fields = new();

	private readonly string _namespaceName = namespaceName;
	private readonly string _typeName = typeName;
	private readonly bool _implicitBase = implicitBase;

}


