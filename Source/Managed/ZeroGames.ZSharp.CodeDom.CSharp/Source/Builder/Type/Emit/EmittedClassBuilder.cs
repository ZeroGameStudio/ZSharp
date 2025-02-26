// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public class EmittedClassBuilder(string namespaceName, string typeName) : GeneratedCompositeTypeBuilderBase<ClassDefinition>(namespaceName, typeName, $"/Script/{namespaceName.Split('.').Last()}.{typeName}")
{
	
	public EmittedEventMethodDefinition AddEventMethod(EMemberVisibility visibility, string name, bool withImplementation, bool withValidation, bool isSealed, TypeReference? returnType, params ParameterDeclaration[]? parameters)
	{
		string zcallName = $"uf://Script/{_namespaceName.Split('.').Last()}.{_typeName}:{name}";
		MethodDefinition stub;
		{
			EMemberModifiers modifiers = EMemberModifiers.Partial;
			stub = new ZCallMethodBuilder(visibility, modifiers, name, zcallName, returnType, false, parameters).Build(false);
			_methods.Add(stub);
		}

		EMemberVisibility implVisibility = isSealed ? EMemberVisibility.Private : EMemberVisibility.Protected;
		EMemberModifiers implModifiers = EMemberModifiers.Partial;
		if (!isSealed)
		{
			implModifiers |= EMemberModifiers.Virtual;
		}
		
		MethodDefinition? implementation = null;
		if (withImplementation)
		{
			implementation = new MethodDefinition(implVisibility, $"{name}_Implementation", returnType, parameters)
			{
				Modifiers = implModifiers,
			};
			implementation.AddAttributeListAfter(new AttributeDeclaration("ZCall"));
			_methods.Add(implementation);
		}
		
		MethodDefinition? validation = null;
		if (withValidation)
		{
			validation = new MethodDefinition(implVisibility, $"{name}_Validate", new("bool", null), parameters)
			{
				Modifiers = implModifiers,
			};
			validation.AddAttributeListAfter(new AttributeDeclaration("ZCall"));
			_methods.Add(validation);
		}

		FieldDefinition zcallHandle = AddStaticField(new("ZCallHandle?", null), $"_zcallHandleFor{name}");

		return new(stub, implementation, validation, zcallHandle);
	}
	
	public EmittedPropertyDefinition AddProperty(EMemberVisibility visibility, TypeReference type, string name, string[]? fieldNotifies, bool needsMarkDirty)
	{
		string zcallName = $"up://Script/{_namespaceName.Split('.').Last()}.{_typeName}:{name}";
		PropertyDefinition property;
		{
			EMemberModifiers modifiers = EMemberModifiers.Partial;
			List<string> beforeSetterReturns = new();
			if (fieldNotifies is not null)
			{
				foreach (var fieldNotify in fieldNotifies)
				{
					beforeSetterReturns.Add($"BroadcastFieldValueChanged({fieldNotify});");
				}
			}
			
			if (needsMarkDirty)
			{
				beforeSetterReturns.Add($"this.MarkPropertyDirty(nameof({name}));");
			}
			
			property = new ZCallPropertyBuilder(visibility, modifiers, name, zcallName, 0, type, false, false)
			{
				BeforeSetterReturnBlock = string.Join(Environment.NewLine, beforeSetterReturns)
			}.Build(false);
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

		foreach (var method in _methods)
		{
			definition.AddMember(method);
		}

		foreach (var property in _properties)
		{
			definition.AddMember(property);
		}

		foreach (var field in _fields)
		{
			definition.AddMember(field);
		}
	}
	
	protected override bool HasBlackConstructor => false;

	protected override string StaticFieldInterfaceName => "IStaticClass";
	protected override string StaticFieldTypeName => "UnrealClass";
	protected override string StaticFieldPropertyName => "StaticClass";
	
	private FieldDefinition AddStaticField(TypeReference type, string name)
	{
		FieldDefinition field = new(EMemberVisibility.Private, name, type)
		{
			Modifiers = EMemberModifiers.Static,
		};
		
		_fields.Add(field);

		return field;
	}
	
	private readonly List<MethodDefinition> _methods = new();
	private readonly List<PropertyDefinition> _properties = new();
	private readonly List<FieldDefinition> _fields = new();

	private readonly string _namespaceName = namespaceName;
	private readonly string _typeName = typeName;
	private readonly string _unrealFieldPath = $"/Script/{namespaceName.Split('.').Last()}.{typeName}";

}


