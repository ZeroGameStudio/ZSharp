// Copyright Zero Games. All Rights Reserved.

using System.Collections.ObjectModel;
using System.Diagnostics;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

partial class ManifestBuilder
{
	
	private void ScanUProperties(UnrealStructDefinition structDef, IUnrealStructModel structModel)
	{
		foreach (var propertyModel in structModel.Properties)
		{
			structDef.Properties.Add(ScanUProperty(structDef, propertyModel));
		}
	}

	private UnrealPropertyDefinition ScanUProperty(UnrealStructDefinition structDef, IUnrealPropertyModel propertyModel)
	{
		TypeModelReference propertyModelType = propertyModel.Type;
		UnrealPropertyDefinition result = new()
		{
			Outer = structDef,
			Name = propertyModel.Name,
			Type = GetPropertyType(propertyModelType, out var descriptorFieldPath),
			DescriptorFieldPath = descriptorFieldPath,
		};
		
		result.InnerProperty = GetInnerProperty(result.Type, propertyModelType);
		result.OuterProperty = GetOuterProperty(result.Type, propertyModelType);

		if (structDef is UnrealFunctionDefinition)
		{
			ScanParameterProperty(result, propertyModel);
		}
		else
		{
			ScanMemberProperty(result, propertyModel);
		}
		
		ProcessSpecifiers(result, propertyModel);
		
		return result;
	}

	private SimpleUnrealPropertyDefinition? GetInnerProperty(EPropertyType outerType, TypeModelReference propertyTypeModel) => outerType switch
	{
		EPropertyType.Array or EPropertyType.Set or EPropertyType.Map or EPropertyType.Optional => ScanContainerElement(propertyTypeModel.GenericArguments![0], outerType == EPropertyType.Set || outerType == EPropertyType.Map),
		_ => null,
	};

	private SimpleUnrealPropertyDefinition? GetOuterProperty(EPropertyType outerType, TypeModelReference propertyTypeModel) => outerType switch
	{
		EPropertyType.Map => ScanContainerElement(propertyTypeModel.GenericArguments![1], false),
		_ => null,
	};

	private SimpleUnrealPropertyDefinition ScanContainerElement(TypeModelReference elementTypeModel, bool throwIfUnhashable)
	{
		SimpleUnrealPropertyDefinition result = new()
		{
			Type = GetPropertyType(elementTypeModel, out var descriptorFieldPath),
			DescriptorFieldPath = descriptorFieldPath,
		};
		
		if (throwIfUnhashable)
		{
			bool hashable = true;
			if (result.Type == EPropertyType.Text || result.Type == EPropertyType.Delegate || result.Type == EPropertyType.MulticastInlineDelegate)
			{
				hashable = false;
			}
			else if (result.Type == EPropertyType.Struct && !elementTypeModel.HasSpecifier<HashableAttribute>())
			{
				hashable = false;
			}
			
			verify(hashable);
		}

		return result;
	}

	private void ScanMemberProperty(UnrealPropertyDefinition result, IUnrealPropertyModel propertyModel)
	{
		check(propertyModel.Role == EPropertyRole.Member);
		
		result.PropertyFlags |= propertyModel.Visibility switch
		{
			EMemberVisibility.Public => EPropertyFlags.CPF_NativeAccessSpecifierPublic,
			EMemberVisibility.Protected => EPropertyFlags.CPF_NativeAccessSpecifierProtected,
			EMemberVisibility.Private => EPropertyFlags.CPF_NativeAccessSpecifierPrivate,
			_ => throw Thrower.NoEntry()
		};
	}

	private void ScanParameterProperty(UnrealPropertyDefinition result, IUnrealPropertyModel propertyModel)
	{
		check(propertyModel.Role != EPropertyRole.Member);
		
		result.PropertyFlags |= EPropertyFlags.CPF_Parm;
		if (propertyModel.Role == EPropertyRole.Parameter)
		{
			if (propertyModel.Type.IsByRef)
			{
				result.PropertyFlags |= EPropertyFlags.CPF_OutParm;
				if (!propertyModel.Type.IsOut)
				{
					result.PropertyFlags |= EPropertyFlags.CPF_ReferenceParm;
				}
			}
		}
		else
		{
			result.PropertyFlags |= EPropertyFlags.CPF_OutParm | EPropertyFlags.CPF_ReturnParm;
		}
	}
	
	private EPropertyType GetPropertyType(TypeModelReference propertyTypeModel, out string? descriptorFieldPath)
	{
		string fullName = propertyTypeModel.FullName;
		
		// Sealed types without descriptor can be resolved directly using FullName so we first do this.
		if (_sealedTypeWithoutDescriptorMap.TryGetValue(fullName, out var result))
		{
			descriptorFieldPath = null;
			return result;
		}

		// Sealed types with descriptor (UObject wrapper types) will use the first generic argument to resolve DescriptorFieldPath
		// so we do this separately with the first step.
		if (_sealedTypeWithDescriptorMap.TryGetValue(fullName, out result))
		{
			check(propertyTypeModel.GenericArguments?.Count == 1);

			descriptorFieldPath = propertyTypeModel.GenericArguments[0].GetUnrealFieldPath();
			return result;
		}
		
		// Finally deal with polymorphism types (UObjects, UStructs, UEnums, UDelegates)
		// and these types must have [UnrealFieldPath] specifier on them.
		if (propertyTypeModel.GetSpecifier<UnrealFieldPathAttribute>() is { } unrealFieldPathSpecifier)
		{
			if (propertyTypeModel.IsEnum())
			{
				result = EPropertyType.Enum;
			}
			
			// Other types must have IStaticXXX interface directly on them.
			foreach (var interfaceTypeUri in propertyTypeModel.Interfaces)
			{
				result = interfaceTypeUri.FullName switch
				{
					STATIC_CLASS_INTERFACE_FULL_NAME => EPropertyType.Object,
					STATIC_STRUCT_INTERFACE_FULL_NAME => EPropertyType.Struct,
					STATIC_SIGNATURE_INTERFACE_FULL_NAME => EPropertyType.Delegate,
					_ => default,
				};

				if (result != default)
				{
					break;
				}
			}

			// There is an exception for object property that TObjectPtr<UClass> is treated as TSubclassOf<UObject>.
			if (result == EPropertyType.Object)
			{
				if (propertyTypeModel.FullName == UNREAL_CLASS_TYPE_FULL_NAME)
				{
					result = EPropertyType.Class;
				}
			}
			// For delegate property, we also need to check whether there is a [Multicast] or [Sparse] specifier on them.
			else if (result == EPropertyType.Delegate)
			{
				foreach (var specifier in propertyTypeModel.Specifiers)
				{
					check(specifier is not SparseAttribute);
					
					if (specifier is MulticastAttribute)
					{
						result = EPropertyType.MulticastInlineDelegate;
						break;
					}
				}
			}

			if (result != default)
			{
				descriptorFieldPath = unrealFieldPathSpecifier.Path;
				return result;
			}
		}

		throw Thrower.NoEntry();
	}

	// Namespaces
	private const string ZSHARP_NAMESPACE = "ZeroGames.ZSharp.";
	private const string ENGINE_NAMESPACE = ZSHARP_NAMESPACE + "UnrealEngine.";
	private const string ENGINE_CORE_UOBJECT_NAMESPACE = ENGINE_NAMESPACE + "CoreUObject.";
	
	// Strings
	private const string UNREAL_STRING_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "UnrealString";
	private const string UNREAL_UTF8_STRING_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "UnrealUtf8String";
	private const string UNREAL_ANSI_STRING_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "UnrealAnsiString";
	private const string UNREAL_NAME_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "UnrealName";
	private const string UNREAL_TEXT_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "UnrealText";
	
	// Object wrappers
	private const string SUBCLASS_OF_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "SubclassOf`1";
	private const string SOFT_CLASS_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "SoftClassPtr`1";
	private const string SOFT_OBJECT_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "SoftObjectPtr`1";
	private const string WEAK_OBJECT_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "WeakObjectPtr`1";
	private const string LAZY_OBJECT_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "LazyObjectPtr`1";
	private const string SCRIPT_INTERFACE_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "ScriptInterface`1";
	
	// Containers
	private const string UNREAL_ARRAY_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "UnrealArray`1";
	private const string UNREAL_SET_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "UnrealSet`1";
	private const string UNREAL_MAP_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "UnrealMap`2";
	private const string UNREAL_OPTIONAL_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "UnrealOptional`1";
	
	// Special types
	private const string FIELD_PATH_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "FieldPath";
	
	// Traits
	private const string UNREAL_CLASS_TYPE_FULL_NAME = ENGINE_CORE_UOBJECT_NAMESPACE + "UnrealClass";
	private const string STATIC_CLASS_INTERFACE_FULL_NAME = ENGINE_NAMESPACE + "IStaticClass";
	private const string STATIC_STRUCT_INTERFACE_FULL_NAME = ENGINE_NAMESPACE + "IStaticStruct";
	private const string STATIC_SIGNATURE_INTERFACE_FULL_NAME = ENGINE_NAMESPACE + "IStaticSignature";

	private static readonly ReadOnlyDictionary<string, EPropertyType> _sealedTypeWithoutDescriptorMap = new(new Dictionary<string, EPropertyType>
	{
		{ typeof(uint8).FullName!, EPropertyType.UInt8 },
		{ typeof(uint16).FullName!, EPropertyType.UInt16 },
		{ typeof(uint32).FullName!, EPropertyType.UInt32 },
		{ typeof(uint64).FullName!, EPropertyType.UInt64 },
		{ typeof(int8).FullName!, EPropertyType.Int8 },
		{ typeof(int16).FullName!, EPropertyType.Int16 },
		{ typeof(int32).FullName!, EPropertyType.Int32 },
		{ typeof(int64).FullName!, EPropertyType.Int64 },
		{ typeof(float).FullName!, EPropertyType.Float },
		{ typeof(double).FullName!, EPropertyType.Double },
		{ typeof(bool).FullName!, EPropertyType.Bool },
		
		{ UNREAL_STRING_TYPE_FULL_NAME, EPropertyType.String },
		{ UNREAL_UTF8_STRING_TYPE_FULL_NAME, EPropertyType.Utf8String },
		{ UNREAL_ANSI_STRING_TYPE_FULL_NAME, EPropertyType.AnsiString },
		{ UNREAL_NAME_TYPE_FULL_NAME, EPropertyType.Name },
		{ UNREAL_TEXT_TYPE_FULL_NAME, EPropertyType.Text },
		
		{ UNREAL_ARRAY_TYPE_FULL_NAME, EPropertyType.Array },
		{ UNREAL_SET_TYPE_FULL_NAME, EPropertyType.Set },
		{ UNREAL_MAP_TYPE_FULL_NAME, EPropertyType.Map },
		{ UNREAL_OPTIONAL_TYPE_FULL_NAME, EPropertyType.Optional },
		
		{ FIELD_PATH_TYPE_FULL_NAME, EPropertyType.FieldPath },
	});

	private static readonly ReadOnlyDictionary<string, EPropertyType> _sealedTypeWithDescriptorMap = new(new Dictionary<string, EPropertyType>
	{
		{ SUBCLASS_OF_TYPE_FULL_NAME, EPropertyType.Class },
		{ SOFT_CLASS_TYPE_FULL_NAME, EPropertyType.SoftClass },
		{ SOFT_OBJECT_TYPE_FULL_NAME, EPropertyType.SoftObject },
		{ WEAK_OBJECT_TYPE_FULL_NAME, EPropertyType.WeakObject },
		{ LAZY_OBJECT_TYPE_FULL_NAME, EPropertyType.LazyObject },
		{ SCRIPT_INTERFACE_TYPE_FULL_NAME, EPropertyType.Interface },
	});

}


