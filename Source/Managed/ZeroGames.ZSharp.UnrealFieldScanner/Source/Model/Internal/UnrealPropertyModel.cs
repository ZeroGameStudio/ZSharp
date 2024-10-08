// Copyright Zero Games. All Rights Reserved.

using Mono.Cecil;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

internal class UnrealPropertyModel : UnrealFieldModel, IUnrealPropertyModel
{

	public UnrealPropertyModel(ModelRegistry registry, PropertyDefinition propertyDef) : base(propertyDef.Name, registry, propertyDef)
	{
		TypeReference typeRef = propertyDef.PropertyType;
		if (typeRef.IsArray || typeRef.IsByReference || typeRef.IsPointer)
		{
			throw new InvalidOperationException();
		}
		
		Role = EPropertyRole.Member;
		Type = new(registry, GetDecayedType(typeRef))
		{
			GenericArguments = ResolveGenericArguments(registry, typeRef),
		};
		
		if (propertyDef.GetMethod is not null)
		{
			Getter = new PropertyAccessorModel(propertyDef.GetMethod, registry);
		}

		if (propertyDef.SetMethod is not null)
		{
			Setter = new PropertyAccessorModel(propertyDef.SetMethod, registry);
		}
	}

	public UnrealPropertyModel(ModelRegistry registry, ParameterDefinition parameterDef) : base(parameterDef.Name, registry, parameterDef)
	{
		TypeReference typeRef = parameterDef.ParameterType;
		if (typeRef.IsArray || typeRef.IsPointer)
		{
			throw new InvalidOperationException();
		}
		
		Role = EPropertyRole.Parameter;
		Type = new(registry, GetDecayedType(typeRef))
		{
			IsByRef = typeRef.IsByReference,
			IsOut = parameterDef.IsOut,
			GenericArguments = ResolveGenericArguments(registry, typeRef),
		};
	}

	public UnrealPropertyModel(ModelRegistry registry, MethodReturnType returnType) : base(returnType.Name, registry, returnType)
	{
		TypeReference typeRef = returnType.ReturnType;
		if (typeRef.IsArray || typeRef.IsByReference || typeRef.IsPointer)
		{
			throw new InvalidOperationException();
		}
		
		Role = EPropertyRole.Return;
		Type = new(registry, GetDecayedType(typeRef))
		{
			GenericArguments = ResolveGenericArguments(registry, typeRef),
		};
	}

	public EPropertyRole Role { get; }
	public TypeModelReference Type { get; }

	public EMemberVisibility Visibility
	{
		get
		{
			if (IsAccessorPublic(Getter) || IsAccessorPublic(Setter))
			{
				return EMemberVisibility.Public;
			}

			if (IsAccessorProtected(Getter) || IsAccessorProtected(Setter))
			{
				return EMemberVisibility.Protected;
			}

			return EMemberVisibility.Private;
		}
	}
	
	public IPropertyAccessorModel? Getter { get; }
	public IPropertyAccessorModel? Setter { get; }

	// Remove reference and generic arguments.
	// Example:
	// int32 -> int32
	// int32& -> int32
	// List<int32> -> List<T>
	// List<int32>& -> List<T>
	private TypeReference GetDecayedType(TypeReference type)
	{
		TypeReference result = type;
		
		/*
		 * Remove reference.
		 * TypeReference.GetElementType() returns wrong value for generic instance type
		 * and we have to use ByReferenceType.ElementType to get the correct GenericInstanceType...
		 */
		if (result is ByReferenceType byRefType)
		{
			result = byRefType.ElementType;
		}

		/*
		 * Remove generic arguments.
		 * For generic instance type, TypeReference.GetElementType() returns the generic type of that instance.
		 * For non generic instance type, TypeReference.GetElementType() just returns itself.
		 */
		return result.GetElementType();
	}

	private List<TypeModelReference>? ResolveGenericArguments(ModelRegistry registry, TypeReference typeRef)
	{
		if (typeRef is ByReferenceType byRefType)
		{
			typeRef = byRefType.ElementType;
		}
		
		if (typeRef is GenericInstanceType genericInstanceType)
		{
			List<TypeModelReference> result = new();
			
			foreach (var genericArgument in genericInstanceType.GenericArguments)
			{
				result.Add(new(registry, genericArgument)
				{
					GenericArguments = ResolveGenericArguments(registry, genericArgument),
				});
			}

			return result;
		}

		return null;
	}

	private bool IsAccessorPublic(IPropertyAccessorModel? accessor) => accessor is not null && accessor.Visibility == EMemberVisibility.Public;
	private bool IsAccessorProtected(IPropertyAccessorModel? accessor) => accessor is not null && accessor.Visibility == EMemberVisibility.Protected;

}


