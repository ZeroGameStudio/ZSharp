﻿// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public enum EExportedClassKind
{
	Class,
	Interface,
	Struct,
	Plain,
}

public class ExportedClassBuilder(bool isAbstraction, EExportedClassKind kind, string namespaceName, string typeName, string? unrealFieldPath, string? baseType) : GeneratedCompositeTypeBuilderBase<ClassDefinition>(namespaceName, typeName, unrealFieldPath)
{
	
	public void AddInterface(string name) => AddBaseType(name);

	public new void AddAttributeBefore(string name, params string[]? arguments) => base.AddAttributeBefore(name, arguments);
	public new void AddAttributeAfter(string name, params string[]? arguments) => base.AddAttributeAfter(name, arguments);
	
	public MethodDefinition AddMethod(EMemberVisibility visibility, bool isVirtual, bool isStatic, string name, string zcallName, TypeReference? returnType, params ParameterDeclaration[]? parameters)
	{
		EMemberModifiers modifiers = EMemberModifiers.Partial;
		if (isVirtual)
		{
			modifiers |= EMemberModifiers.Virtual;
		}

		if (isStatic)
		{
			modifiers |= EMemberModifiers.Static;
		}
		
		MethodDefinition method = new ZCallMethodBuilder(visibility, modifiers, name, zcallName, returnType, false, parameters).Build(IsAbstraction);
		_methods.Add(method);

		return method;
	}

	public MethodDefinition AddPureVirtualMethod(EMemberVisibility visibility, string name, TypeReference? returnType, params ParameterDeclaration[]? parameters)
	{
		MethodDefinition method = new(visibility, name, returnType, parameters)
		{
			Modifiers = EMemberModifiers.Virtual,
			IsPureVirtual = true,
		};
		_methods.Add(method);

		return method;
	}

	public PropertyDefinition AddProperty(EMemberVisibility visibility, TypeReference type, string name, string zcallName, int32 index, bool readOnly, bool allowNull)
	{
		EMemberModifiers modifiers = EMemberModifiers.Partial;

		PropertyDefinition property = new ZCallPropertyBuilder(visibility, modifiers, name, zcallName, index, type, false, readOnly).Build(IsAbstraction);
		if (!readOnly && allowNull)
		{
			property.AddAttributeListAfter(new AttributeDeclaration("AllowNull"));
		}
		_properties.Add(property);

		return property;
	}

	// There may be multiple overloads of one ZCall so we add this interface.
	public FieldDefinition AddStaticFieldIfNotExists(TypeReference type, string name)
	{
		if (_fields.Find(field => field.Name == name) is {} existingField)
		{
			return existingField;
		}
		
		return AddStaticField(type, name);
	}

	public FieldDefinition AddStaticField(TypeReference type, string name)
	{
		FieldDefinition field = new(EMemberVisibility.Private, name, type)
		{
			Modifiers = EMemberModifiers.Static,
		};
		
		_fields.Add(field);

		return field;
	}

	public EExportedClassKind Kind { get; } = kind;
	public bool IsAbstraction { get; } = isAbstraction;
	public string? BaseType { get; } = baseType;

	protected override ClassDefinition AllocateTypeDefinition() => new(Kind == EExportedClassKind.Interface, EMemberVisibility.Public, TypeName);

	protected override void BuildTypeDefinition(ClassDefinition definition)
	{
		base.BuildTypeDefinition(definition);

		if (!IsAbstraction)
		{
			definition.Modifiers |= EMemberModifiers.Unsafe;
		}

		if (IsAbstraction && BaseType is not null)
		{
			definition.AddBaseType(BaseType);
		}

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

	protected override string GetBlackConstructorBody()
	{
		if (!HasBlackConstructor)
		{
			return base.GetBlackConstructorBody();
		}
		
		string userdata = Kind == EExportedClassKind.Struct ? "StaticStruct.Unmanaged" : "IntPtr.Zero";
		return $" {{ Unmanaged = MasterAlcCache.Instance.BuildConjugate(this, {userdata}); }}";
	}

	protected override bool GenerateIntrinsicContent => !IsAbstraction;

	protected override bool HasUnrealFieldPath => Kind != EExportedClassKind.Plain;
	protected override bool HasUnrealFieldPathConst => Kind != EExportedClassKind.Plain;

	protected override bool HasBuildConjugate => Kind != EExportedClassKind.Interface;

	protected override bool HasBlackConstructor => Kind != EExportedClassKind.Class && Kind != EExportedClassKind.Interface;
	protected override bool HasRedConstructor => Kind != EExportedClassKind.Interface;

	protected override string StaticFieldInterfaceName => Kind == EExportedClassKind.Struct ? "IStaticStruct" : Kind != EExportedClassKind.Plain ? "IStaticClass" : throw new InvalidOperationException();
	protected override string StaticFieldTypeName => Kind == EExportedClassKind.Struct ? "UnrealScriptStruct" : Kind != EExportedClassKind.Plain ? "UnrealClass" : throw new InvalidOperationException();
	protected override string StaticFieldPropertyName => Kind == EExportedClassKind.Struct ? "StaticStruct" : Kind != EExportedClassKind.Plain ? "StaticClass" : throw new InvalidOperationException();

	private readonly List<MethodDefinition> _methods = new();
	private readonly List<PropertyDefinition> _properties = new();
	private readonly List<FieldDefinition> _fields = new();

}

