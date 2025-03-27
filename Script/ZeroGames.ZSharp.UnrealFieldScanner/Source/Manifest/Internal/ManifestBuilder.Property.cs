// Copyright Zero Games. All Rights Reserved.

using System.Collections.ObjectModel;

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
			Type = GetPropertyType(propertyModelType, out var enumUnderlyingType, out var descriptorFieldPath),
			EnumUnderlyingType = enumUnderlyingType,
			DescriptorFieldPath = descriptorFieldPath,
		};

		if (structDef is not IUnrealSignatureDefinition)
		{
			result.AddMetadata("Category", structDef.Name);
		}
		
		ProcessSpecifiers(result, propertyModel);
		
		result.InnerProperty = GetInnerProperty(result.Type, propertyModelType);
		result.OuterProperty = GetOuterProperty(result.Type, propertyModelType);

		if (structDef is IUnrealSignatureDefinition)
		{
			ScanParameterProperty(result, propertyModel);
		}
		else
		{
			ScanMemberProperty(result, propertyModel);
		}
		
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
			Type = GetPropertyType(elementTypeModel, out var enumUnderlyingType, out var descriptorFieldPath),
			EnumUnderlyingType = enumUnderlyingType,
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
			EMemberVisibility.Protected => EPropertyFlags.CPF_NativeAccessSpecifierProtected | EPropertyFlags.CPF_Protected,
			EMemberVisibility.Private => EPropertyFlags.CPF_NativeAccessSpecifierPrivate,
			_ => throw Thrower.NoEntry()
		};
	}

	private void ScanParameterProperty(UnrealPropertyDefinition result, IUnrealPropertyModel propertyModel)
	{
		check(propertyModel.Role != EPropertyRole.Member);
		
		result.PropertyFlags |= EPropertyFlags.CPF_Parm | EPropertyFlags.CPF_NativeAccessSpecifierPublic; // Parameter is always public. (@see: UhtPropertyParser.cs)
		if (propertyModel.Role == EPropertyRole.Parameter)
		{
			bool output = propertyModel.Type.IsByRef;
			if (output)
			{
				result.PropertyFlags |= EPropertyFlags.CPF_OutParm;
				if (!propertyModel.Type.IsOut)
				{
					result.PropertyFlags |= EPropertyFlags.CPF_ReferenceParm;
				}
			}
			// These types of input are passed by const&.
			else if (result.Type
			         is EPropertyType.String
			         or EPropertyType.AnsiString
			         or EPropertyType.Utf8String
			         or EPropertyType.Name
			         or EPropertyType.Text
			         or EPropertyType.WeakObject
			         or EPropertyType.SoftObject
			         or EPropertyType.LazyObject
			         or EPropertyType.SoftClass
			         or EPropertyType.Interface
			         or EPropertyType.Struct
			         or EPropertyType.Array
			         or EPropertyType.Set
			         or EPropertyType.Map
			         or EPropertyType.Optional
			         or EPropertyType.Delegate
			         or EPropertyType.MulticastInlineDelegate)
			{
				// String properties don't have these. (@see: UhtXXXStrProperty constructor)
				if (result.Type is not (EPropertyType.String or EPropertyType.AnsiString or EPropertyType.Utf8String))
				{
					result.PropertyFlags |= EPropertyFlags.CPF_ConstParm | EPropertyFlags.CPF_ReferenceParm;
					// Non replicated parameter will also have CPF_OutParm. (@see: UhtPropertyParser.ResolvePropertyType)
					if (result.Outer is not UnrealFunctionDefinition function || !function.FunctionFlags.HasFlag(EFunctionFlags.FUNC_Net))
					{
						result.PropertyFlags |= EPropertyFlags.CPF_OutParm;
					}
				}

				result.AddMetadata(MetadataConstants.NativeConst);
			}
		}
		else
		{
			result.PropertyFlags |= EPropertyFlags.CPF_OutParm | EPropertyFlags.CPF_ReturnParm;
		}
	}
	
	private EPropertyType GetPropertyType(TypeModelReference propertyTypeModel, out EEnumUnderlyingType enumUnderlyingType, out string? descriptorFieldPath)
	{
		enumUnderlyingType = EEnumUnderlyingType.None;
		
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
				enumUnderlyingType = TypeUriToEnumUnderlyingType(propertyTypeModel.Type.EnumUnderlyingType);
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

			// For delegate property, we need a further check.
			if (result == EPropertyType.Delegate)
			{
				result = propertyTypeModel.BaseType!.Value.FullName switch
				{
					UNREAL_DELEGATE_BASE_TYPE_FULL_NAME => EPropertyType.Delegate,
					UNREAL_MULTICAST_INLINE_DELEGATE_BASE_TYPE_FULL_NAME => EPropertyType.MulticastInlineDelegate,
					UNREAL_MULTICAST_SPARSE_DELEGATE_BASE_TYPE_FULL_NAME => EPropertyType.MulticastSparseDelegate,
					_ => default,
				};
			}

			if (result != default)
			{
				descriptorFieldPath = unrealFieldPathSpecifier.Path;
				return result;
			}
		}

		throw Thrower.NoEntry();
	}

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


