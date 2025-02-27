﻿// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public class EmittedScriptStructBuilder(string namespaceName, string typeName) : GeneratedCompositeTypeBuilderBase<ClassDefinition>(namespaceName, typeName, $"/Script/{namespaceName.Split('.').Last()}.{typeName}")
{
	
	public EmittedPropertyDefinition AddProperty(EMemberVisibility visibility, TypeReference type, string name)
	{
		string zcallName = $"up://Script/{_namespaceName.Split('.').Last()}.{_typeName}:{name}";
		PropertyDefinition property;
		{
			EMemberModifiers modifiers = EMemberModifiers.Partial;
			property = new ZCallPropertyBuilder(visibility, modifiers, name, zcallName, 0, type, false, false).Build(false);
			_properties.Add(property);
		}
		
		FieldDefinition zcallHandle = AddStaticField(new("ZCallHandle?", null), $"_zcallHandleFor{name}");

		return new(property, zcallHandle);
	}
	
	protected override ClassDefinition AllocateTypeDefinition() => new(false, EMemberVisibility.Public, TypeName);

	protected override void BuildTypeDefinition(ClassDefinition definition)
	{
		base.BuildTypeDefinition(definition);

		definition.Modifiers |= EMemberModifiers.Unsafe;
		
		AddAttributeAfter("ConjugateKey", $"\"{_unrealFieldPath}\"");

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
		=> $" {{ if (GetType() == typeof({TypeName})) Unmanaged = MasterAlcCache.Instance.BuildConjugate(this, StaticStruct.Unmanaged); }}";

	protected override string StaticFieldInterfaceName => "IStaticStruct";
	protected override string StaticFieldTypeName => "UnrealScriptStruct";
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
	private readonly string _unrealFieldPath = $"/Script/{namespaceName.Split('.').Last()}.{typeName}";
	
}


