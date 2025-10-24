// Copyright Zero Games. All Rights Reserved.

using Mono.Cecil;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

internal class UnrealPropertyModel : UnrealFieldModel, IUnrealPropertyModel
{

	public UnrealPropertyModel(ModelRegistry registry, PropertyDefinition propertyDef, IUnrealStructModel outer) : base(propertyDef.Name, registry, propertyDef)
	{
		TypeReference typeRef = propertyDef.PropertyType;
		check(!typeRef.IsArray);
		check(!typeRef.IsByReference);
		check(!typeRef.IsPointer);
		
		Outer = outer;
		Role = EPropertyRole.Member;
		Type = new(registry, typeRef);
		
		if (propertyDef.GetMethod is not null)
		{
			Getter = new PropertyAccessorModel(propertyDef.GetMethod, registry);
		}

		if (propertyDef.SetMethod is not null)
		{
			Setter = new PropertyAccessorModel(propertyDef.SetMethod, registry);
		}
	}

	public UnrealPropertyModel(ModelRegistry registry, ParameterDefinition parameterDef, IUnrealStructModel outer) : base(GetParameterName(parameterDef), registry, parameterDef)
	{
		TypeReference typeRef = parameterDef.ParameterType;
		check(!typeRef.IsArray);
		check(!typeRef.IsPointer);

		Outer = outer;
		Role = EPropertyRole.Parameter;
		Type = new(registry, typeRef)
		{
			IsByRef = typeRef.IsByReference,
			IsOut = parameterDef.IsOut,
		};
	}

	public UnrealPropertyModel(ModelRegistry registry, MethodReturnType returnType, IUnrealStructModel outer) : base(RETURN_NAME, registry, returnType)
	{
		TypeReference typeRef = returnType.ReturnType;
		check(!typeRef.IsArray);
		check(!typeRef.IsByReference);
		check(!typeRef.IsPointer);

		Outer = outer;
		Role = EPropertyRole.Return;
		Type = new(registry, typeRef);
	}

	public IUnrealStructModel Outer { get; }
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

	private static string GetParameterName(ParameterDefinition parameterDef)
	{
		string? name = null;
		if (parameterDef.CustomAttributes.SingleOrDefault(attr => attr.AttributeType.FullName == typeof(UParamAttribute).FullName) is { } uparamSpecifier)
		{
			if (uparamSpecifier.Properties.SingleOrDefault(prop => prop.Name == nameof(UParamAttribute.Name)) is { } nameProperty)
			{
				name = nameProperty.Argument.Value?.ToString();
			}
		}

		return !string.IsNullOrEmpty(name) ? name : parameterDef.Name;
	}
	
	private bool IsAccessorPublic(IPropertyAccessorModel? accessor) => accessor is not null && accessor.Visibility == EMemberVisibility.Public;
	private bool IsAccessorProtected(IPropertyAccessorModel? accessor) => accessor is not null && accessor.Visibility == EMemberVisibility.Protected;
	
	private const string RETURN_NAME = "ReturnValue";

}


