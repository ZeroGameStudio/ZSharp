// Copyright Zero Games. All Rights Reserved.

using Mono.Cecil;
using ZeroGames.ZSharp.Emit.Specifier;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

partial class UnrealFieldScanner
{

	private void ScanUProperties(TypeDefinition type, UnrealStructDefinition strct)
	{
		foreach (var prop in type.Properties.Where(HasCustomAttribute<UPropertyAttribute>))
		{
			if (GetCustomAttributeOrDefault<UPropertyAttribute>(prop) is { } upropertyAttr)
			{
				ScanUProperty(prop, strct, upropertyAttr);
			}
		}
	}

	private void ScanUProperty(PropertyDefinition prop, UnrealStructDefinition strct, CustomAttribute upropertyAttr)
	{
		TypeReference propertyType = prop.PropertyType;
		if (propertyType.IsArray)
		{
			throw new NotSupportedException("UProperty array is not supported.");
		}
		
		if (propertyType.IsByReference)
		{
			throw new NotSupportedException("UProperty reference is not supported.");
		}
		
		if (propertyType.IsPointer)
		{
			throw new NotSupportedException("UProperty pointer is not supported.");
		}
		
		UnrealPropertyDefinition def = new()
		{
			Name = prop.Name,
			Type = GetPropertyType(propertyType, out var descriptorFieldPath),
			DescriptorFieldPath = descriptorFieldPath,
		};

		if (propertyType is GenericInstanceType genericInstanceType)
		{
			ScanElementProperties(genericInstanceType, def);
		}

		{ // Default
			if (strct is UnrealClassDefinition cls)
			{
				if (TryGetAttributePropertyValue<object>(upropertyAttr, nameof(UPropertyAttribute.Default), out var value))
				{
					cls.PropertyDefaults.Add(new ()
					{
						PropertyChain = def.Name,
						Buffer = value.ToString()!,
					});
				}

				if (GetCustomAttributeOrDefault<DefaultSubobjectAttribute>(prop) is {} defaultSubobjectAttr)
				{
					TryGetAttributePropertyValue<string>(defaultSubobjectAttr, nameof(DefaultSubobjectAttribute.Name), out var name);
					name ??= def.Name;

					string? parent = default;
					string? socket = default;
					if (GetCustomAttributeOrDefault<AttachmentAttribute>(prop) is { } attachmentAttr)
					{
						TryGetAttributePropertyValue(attachmentAttr, nameof(AttachmentAttribute.Parent), out parent);
						TryGetAttributePropertyValue(attachmentAttr, nameof(AttachmentAttribute.Socket), out socket);
					}
					
					cls.DefaultSubobjects.Add(new ()
					{
						Name = name,
						ClassPath = "", // @TODO
						IsTransient = false, // @TODO
						PropertyName = def.Name,
						AttachParentDefaultSubobjectName = parent,
						AttachSocketName = socket,
					});
				}
				else if (GetCustomAttributeOrDefault<OptionalDefaultSubobjectAttribute>(prop) is {} optionalDefaultSubobjectAttr)
				{
					TryGetAttributePropertyValue<string>(optionalDefaultSubobjectAttr, nameof(DefaultSubobjectAttribute.Name), out var name);
					name ??= def.Name;
					
					string? parent = default;
					string? socket = default;
					if (GetCustomAttributeOrDefault<AttachmentAttribute>(prop) is { } attachmentAttr)
					{
						TryGetAttributePropertyValue(attachmentAttr, nameof(AttachmentAttribute.Parent), out parent);
						TryGetAttributePropertyValue(attachmentAttr, nameof(AttachmentAttribute.Socket), out socket);
					}
					
					cls.DefaultSubobjects.Add(new ()
					{
						Name = name,
						ClassPath = "", // @TODO
						IsOptional = true,
						IsTransient = false, // @TODO
						PropertyName = def.Name,
						AttachParentDefaultSubobjectName = parent,
						AttachSocketName = socket,
					});
				}
				else if (GetCustomAttributeOrDefault<RootComponentAttribute>(prop) is {} rootComponentAttr)
				{
					TryGetAttributePropertyValue<string>(rootComponentAttr, nameof(DefaultSubobjectAttribute.Name), out var name);
					name ??= def.Name;
					cls.DefaultSubobjects.Add(new ()
					{
						Name = name,
						ClassPath = "", // @TODO
						IsTransient = false, // @TODO
						PropertyName = def.Name,
						IsRootComponent = true,
					});
				}
			}
		}

		{ // Native visibility
			MethodDefinition getter = prop.GetMethod;
			if (getter.IsPublic)
			{
				def.PropertyFlags |= EPropertyFlags.CPF_NativeAccessSpecifierPublic;
			}
			else
			{
				def.PropertyFlags |= EPropertyFlags.CPF_NativeAccessSpecifierPrivate;
			}
		}

		{ // Blueprint visibility
			if (HasCustomAttribute<BlueprintReadWriteAttribute>(prop))
			{
				def.PropertyFlags |= EPropertyFlags.CPF_BlueprintVisible;
			}
			else if (HasCustomAttribute<BlueprintReadOnlyAttribute>(prop))
			{
				def.PropertyFlags |= EPropertyFlags.CPF_BlueprintVisible;
				def.PropertyFlags |= EPropertyFlags.CPF_BlueprintReadOnly;
			}
		}
		
		{ // Edit
			if (HasCustomAttribute<EditAnywhereAttribute>(prop))
			{
				def.PropertyFlags |= EPropertyFlags.CPF_Edit;
			}
			else if (HasCustomAttribute<EditInstanceOnlyAttribute>(prop))
			{
				def.PropertyFlags |= EPropertyFlags.CPF_Edit;
				def.PropertyFlags |= EPropertyFlags.CPF_DisableEditOnTemplate;
			}
			else if (HasCustomAttribute<EditDefaultsOnlyAttribute>(prop))
			{
				def.PropertyFlags |= EPropertyFlags.CPF_Edit;
				def.PropertyFlags |= EPropertyFlags.CPF_DisableEditOnInstance;
			}
			else if (HasCustomAttribute<VisibleAnywhereAttribute>(prop))
			{
				def.PropertyFlags |= EPropertyFlags.CPF_Edit;
				def.PropertyFlags |= EPropertyFlags.CPF_EditConst;
			}
			else if (HasCustomAttribute<VisibleInstanceOnlyAttribute>(prop))
			{
				def.PropertyFlags |= EPropertyFlags.CPF_Edit;
				def.PropertyFlags |= EPropertyFlags.CPF_EditConst;
				def.PropertyFlags |= EPropertyFlags.CPF_DisableEditOnTemplate;
			}
			else if (HasCustomAttribute<VisibleDefaultsOnlyAttribute>(prop))
			{
				def.PropertyFlags |= EPropertyFlags.CPF_Edit;
				def.PropertyFlags |= EPropertyFlags.CPF_EditConst;
				def.PropertyFlags |= EPropertyFlags.CPF_DisableEditOnInstance;
			}
		}
		
		if (_withMetadata)
		{
			ScanMetadata(prop, def);
		}

		strct.Properties.Add(def);
	}

	private void ScanUParams(MethodDefinition method, UnrealFunctionDefinition function)
	{
		foreach (var param in method.Parameters)
		{
			ScanUParam(param, function);
		}
	}

	private void ScanUParam(ParameterDefinition param, UnrealFunctionDefinition function)
	{
		TypeReference parameterType = param.ParameterType;
		if (parameterType.IsArray)
		{
			throw new NotSupportedException("UParam array is not supported.");
		}
		
		if (parameterType.IsPointer)
		{
			throw new NotSupportedException("UParam pointer is not supported.");
		}
		
		UnrealPropertyDefinition def = new()
		{
			Name = param.Name,
			Type = GetPropertyType(parameterType, out var descriptorFieldPath),
			DescriptorFieldPath = descriptorFieldPath,
		};

		// TypeReference.GetElementType() returns a TypeReference and we must use ByReferenceType.ElementType to get the GenericInstanceType.
		TypeReference genericInstanceTypeToTest = parameterType is ByReferenceType refParameterType ? refParameterType.ElementType : parameterType;
		if (genericInstanceTypeToTest is GenericInstanceType genericInstanceType)
		{
			ScanElementProperties(genericInstanceType, def);
		}

		def.PropertyFlags |= EPropertyFlags.CPF_Parm;
		if (param.IsReturnValue)
		{
			def.PropertyFlags |= EPropertyFlags.CPF_OutParm;
			def.PropertyFlags |= EPropertyFlags.CPF_ReturnParm;
		}
		else if (parameterType.IsByReference)
		{
			function.FunctionFlags |= EFunctionFlags.FUNC_HasOutParms;
			def.PropertyFlags |= EPropertyFlags.CPF_OutParm;
			if (!param.IsOut)
			{
				def.PropertyFlags |= EPropertyFlags.CPF_ReferenceParm;
			}
		}

		if (_withMetadata)
		{
			ScanMetadata(param, def);
		}

		function.Properties.Add(def);
	}

	private void ScanUReturnParam(TypeReference parameterType, UnrealFunctionDefinition function)
	{
		if (parameterType.IsArray)
		{
			throw new NotSupportedException("UReturnParam array is not supported.");
		}
		
		if (parameterType.IsByReference)
		{
			throw new NotSupportedException("UReturnParam reference is not supported.");
		}
		
		if (parameterType.IsPointer)
		{
			throw new NotSupportedException("UReturnParam pointer is not supported.");
		}
		
		UnrealPropertyDefinition def = new()
		{
			Name = RETURN_PARAM_NAME,
			Type = GetPropertyType(parameterType, out var descriptorFieldPath),
			DescriptorFieldPath = descriptorFieldPath,
		};
		
		if (parameterType is GenericInstanceType genericInstanceType)
		{
			ScanElementProperties(genericInstanceType, def);
		}

		def.PropertyFlags |= EPropertyFlags.CPF_Parm;
		def.PropertyFlags |= EPropertyFlags.CPF_OutParm;
		def.PropertyFlags |= EPropertyFlags.CPF_ReturnParm;

		function.Properties.Add(def);
	}

	private void ScanElementProperties(GenericInstanceType outerPropertyType, UnrealPropertyDefinition outerProp)
	{
		switch (outerProp.Type)
		{
			case EPropertyType.Array:
			case EPropertyType.Optional:
			{
				ScanElementProperty(outerPropertyType.GenericArguments[0], outerProp, false);
				break;
			}
			case EPropertyType.Set:
			{
				ScanElementProperty(outerPropertyType.GenericArguments[0], outerProp, true);
				break;
			}
			case EPropertyType.Map:
			{
				ScanElementProperty(outerPropertyType.GenericArguments[0], outerProp, true);
				ScanElementProperty(outerPropertyType.GenericArguments[1], outerProp, false);
				break;
			}
			default:
			{
				break;
			}
		}
	}

	private void ScanElementProperty(TypeReference elementTypeRef, UnrealPropertyDefinition outerProp, bool requiresHashable)
	{
		SimpleUnrealPropertyDefinition elementProp = new()
		{
			Type = GetPropertyType(elementTypeRef, out var descriptorFieldPath),
			DescriptorFieldPath = descriptorFieldPath,
		};

		if (requiresHashable)
		{
			bool hashable = true;
			if (elementProp.Type == EPropertyType.Text || elementProp.Type == EPropertyType.Delegate || elementProp.Type == EPropertyType.MulticastInlineDelegate)
			{
				hashable = false;
			}
			else if (elementProp.Type == EPropertyType.Struct && !HasCustomAttribute(GetTypeDefinition(elementTypeRef), HASHABLE_ATTRIBUTE_FULL_NAME))
			{
				hashable = false;
			}

			if (!hashable)
			{
				throw new InvalidOperationException();
			}
		}

		if (outerProp.InnerProperty is null)
		{
			outerProp.InnerProperty = elementProp;
		}
		else if (outerProp.OuterProperty is null)
		{
			outerProp.OuterProperty = elementProp;
		}
		else
		{
			throw new InvalidOperationException();
		}
	}

	private EPropertyType GetPropertyType(TypeReference propertyTypeRef, out string? descriptorFieldPath)
	{
		descriptorFieldPath = null;

		if (propertyTypeRef is ByReferenceType refPropertyTypeRef)
		{
			// TypeReference.GetElementType() returns a TypeReference and we must use ByReferenceType.ElementType to get the GenericInstanceType.
			propertyTypeRef = refPropertyTypeRef.ElementType;
		}
		// For generic instance type, this is the generic type name, i.e. ZeroGames.ZSharp.UnrealEngine.Core.UnrealArray`1.
		// For normal type, this is the same as propertyTypeRef.FullName.
		string propertyTypeName = propertyTypeRef.GetElementType().FullName;
		
		EPropertyType type = propertyTypeName switch
		{
			// Primitives
			UINT8_TYPE_FULL_NAME => EPropertyType.UInt8,
			UINT16_TYPE_FULL_NAME => EPropertyType.UInt16,
			UINT32_TYPE_FULL_NAME => EPropertyType.UInt32,
			UINT64_TYPE_FULL_NAME => EPropertyType.UInt64,
			INT8_TYPE_FULL_NAME => EPropertyType.Int8,
			INT16_TYPE_FULL_NAME => EPropertyType.Int16,
			INT32_TYPE_FULL_NAME => EPropertyType.Int32,
			INT64_TYPE_FULL_NAME => EPropertyType.Int64,
			FLOAT_TYPE_FULL_NAME => EPropertyType.Float,
			DOUBLE_TYPE_FULL_NAME => EPropertyType.Double,
			BOOL_TYPE_FULL_NAME => EPropertyType.Bool,
			// Strings
			STRING_TYPE_FULL_NAME => EPropertyType.String, // Special support for System.String.
			UNREAL_STRING_TYPE_FULL_NAME => EPropertyType.String,
			UNREAL_NAME_TYPE_FULL_NAME => EPropertyType.Name,
			UNREAL_TEXT_TYPE_FULL_NAME => EPropertyType.Text,
			// Non-struct containers
			UNREAL_ARRAY_TYPE_FULL_NAME => EPropertyType.Array,
			UNREAL_SET_TYPE_FULL_NAME => EPropertyType.Set,
			UNREAL_MAP_TYPE_FULL_NAME => EPropertyType.Map,
			UNREAL_OPTIONAL_TYPE_FULL_NAME => EPropertyType.Optional,
			// Special types
			FIELD_PATH_TYPE_FULL_NAME => EPropertyType.FieldPath,
			_ => default
		};

		if (type != default)
		{
			return type;
		}
		
		// Object wrappers
		{
			type = propertyTypeName switch
			{
				UNREAL_CLASS_TYPE_FULL_NAME => EPropertyType.Class,
				SUBCLASS_OF_TYPE_FULL_NAME => EPropertyType.Class,
				SOFT_CLASS_TYPE_FULL_NAME => EPropertyType.SoftClass,
				SOFT_OBJECT_TYPE_FULL_NAME => EPropertyType.SoftObject,
				WEAK_OBJECT_TYPE_FULL_NAME => EPropertyType.WeakObject,
				LAZY_OBJECT_TYPE_FULL_NAME => EPropertyType.LazyObject,
				SCRIPT_INTERFACE_TYPE_FULL_NAME => EPropertyType.Interface,
				_ => default,
			};

			if (type != default)
			{
				TypeReference descriptorType = propertyTypeRef is GenericInstanceType genericInstanceType ? genericInstanceType.GenericArguments[0] : propertyTypeRef;
				descriptorFieldPath = GetUnrealFieldPath(descriptorType);
				return type;
			}
		}
		
		// UEnum, UObject, UStruct, UDelegate
		if (GetUnrealFieldPathOrDefault(propertyTypeRef) is { } unrealFieldPath)
		{
			TypeDefinition propertyType = GetTypeDefinition(propertyTypeRef);
			
			if (propertyType.IsEnum)
			{
				type = EPropertyType.Enum;
			}

			foreach (var i in propertyType.Interfaces)
			{
				type = i.InterfaceType.FullName switch
				{
					STATIC_CLASS_INTERFACE_FULL_NAME => EPropertyType.Object,
					STATIC_STRUCT_INTERFACE_FULL_NAME => EPropertyType.Struct,
					STATIC_SIGNATURE_INTERFACE_FULL_NAME => EPropertyType.Delegate,
					_ => default,
				};
			}

			if (type == EPropertyType.Delegate)
			{
				foreach (var attr in propertyType.CustomAttributes)
				{
					string attrTypeFullName = attr.AttributeType.FullName;
					if (attrTypeFullName == MULTICAST_ATTRIBUTE_FULL_NAME)
					{
						type = EPropertyType.MulticastInlineDelegate;
					}
					else if (attrTypeFullName == SPARSE_ATTRIBUTE_FULL_NAME)
					{
						// Multicast sparse delegate is not supported.
						type = default;
					}
				}
			}

			if (type != default)
			{
				descriptorFieldPath = unrealFieldPath;
				return type;
			}
		}

		throw new NotSupportedException($"Property type [{propertyTypeRef.FullName}] is not supported.");
	}

	private const string SYSTEM_NAMESPACE = "System.";
	private const string ENGINE_NAMESPACE = "ZeroGames.ZSharp.UnrealEngine.";
	private const string ENGINE_CORE_NAMESPACE = ENGINE_NAMESPACE + "Core.";
	private const string ENGINE_CORE_UOBJECT_NAMESPACE = ENGINE_NAMESPACE + "CoreUObject.";

	// Primitives
	private const string UINT8_TYPE_FULL_NAME = SYSTEM_NAMESPACE + nameof(Byte);
	private const string UINT16_TYPE_FULL_NAME = SYSTEM_NAMESPACE + nameof(UInt16);
	private const string UINT32_TYPE_FULL_NAME = SYSTEM_NAMESPACE + nameof(UInt32);
	private const string UINT64_TYPE_FULL_NAME = SYSTEM_NAMESPACE + nameof(UInt64);
	private const string INT8_TYPE_FULL_NAME = SYSTEM_NAMESPACE + nameof(SByte);
	private const string INT16_TYPE_FULL_NAME = SYSTEM_NAMESPACE + nameof(Int16);
	private const string INT32_TYPE_FULL_NAME = SYSTEM_NAMESPACE + nameof(Int32);
	private const string INT64_TYPE_FULL_NAME = SYSTEM_NAMESPACE + nameof(Int64);
	private const string FLOAT_TYPE_FULL_NAME = SYSTEM_NAMESPACE + nameof(Single);
	private const string DOUBLE_TYPE_FULL_NAME = SYSTEM_NAMESPACE + nameof(Double);
	private const string BOOL_TYPE_FULL_NAME = SYSTEM_NAMESPACE + nameof(Boolean);

	// Strings
	private const string STRING_TYPE_FULL_NAME = SYSTEM_NAMESPACE + nameof(String);
	private const string UNREAL_STRING_TYPE_FULL_NAME = ENGINE_CORE_NAMESPACE + "UnrealString";
	private const string UNREAL_NAME_TYPE_FULL_NAME = ENGINE_CORE_NAMESPACE + "UnrealName";
	private const string UNREAL_TEXT_TYPE_FULL_NAME = ENGINE_CORE_NAMESPACE + "UnrealText";
	
	// Object wrappers
	private const string UNREAL_CLASS_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "UnrealClass";
	private const string SUBCLASS_OF_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "SubclassOf`1";
	private const string SOFT_CLASS_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "SoftClassPtr`1";
	private const string SOFT_OBJECT_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "SoftObjectPtr`1";
	private const string WEAK_OBJECT_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "WeakObjectPtr`1";
	private const string LAZY_OBJECT_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "LazyObjectPtr`1";
	private const string SCRIPT_INTERFACE_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "ScriptInterface`1";
	
	// Containers
	private const string UNREAL_ARRAY_TYPE_FULL_NAME = ENGINE_CORE_NAMESPACE + "UnrealArray`1";
	private const string UNREAL_SET_TYPE_FULL_NAME = ENGINE_CORE_NAMESPACE + "UnrealSet`1";
	private const string UNREAL_MAP_TYPE_FULL_NAME = ENGINE_CORE_NAMESPACE + "UnrealMap`2";
	private const string UNREAL_OPTIONAL_TYPE_FULL_NAME = ENGINE_CORE_NAMESPACE + "UnrealOptional`1";
	
	// Special types
	private const string FIELD_PATH_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "FieldPath";
	
	// Unreal field traits
	private const string STATIC_CLASS_INTERFACE_FULL_NAME = ENGINE_NAMESPACE + "IStaticClass";
	private const string STATIC_STRUCT_INTERFACE_FULL_NAME = ENGINE_NAMESPACE + "IStaticStruct";
	private const string STATIC_SIGNATURE_INTERFACE_FULL_NAME = ENGINE_NAMESPACE + "IStaticSignature";
	private const string HASHABLE_ATTRIBUTE_FULL_NAME = ENGINE_NAMESPACE + "HashableAttribute";
	private const string MULTICAST_ATTRIBUTE_FULL_NAME = ENGINE_NAMESPACE + "MulticastAttribute";
	private const string SPARSE_ATTRIBUTE_FULL_NAME = ENGINE_NAMESPACE + "SparseAttribute";

	// Misc
	private const string RETURN_PARAM_NAME = "ReturnValue";

}


