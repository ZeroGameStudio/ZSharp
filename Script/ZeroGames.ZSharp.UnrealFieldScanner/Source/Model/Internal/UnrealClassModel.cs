// Copyright Zero Games. All Rights Reserved.

using Mono.Cecil;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

internal class UnrealClassModel : UnrealStructModel, IUnrealClassModel, IDeferredTypeModel
{

	public UnrealClassModel(ModelRegistry registry, TypeDefinition typeDef) : base(typeDef.Name)
	{
		_registry = registry;
		_typeDef = typeDef;

		AssemblyName = typeDef.Scope.GetAssemblyName();
		FullName = typeDef.FullName;

		MethodDefinition? maybeCtor = typeDef.Methods.SingleOrDefault(method => method.HasCustomAttribute<UClassConstructorAttribute>());
		if (maybeCtor is { HasParameters: true } or { IsStatic: true })
		{
			throw new InvalidOperationException();
		}
		
		HasUClassConstructor = maybeCtor is not null;
		IsInternal = typeDef.IsNotPublic;
	}

	void IDeferredTypeModel.BaseInitialize()
	{
		if (IsBaseInitialized)
		{
			return;
		}
		
		SpecifierResolver.Resolve(_registry, _typeDef, _specifiers);

		_baseType = new(_registry, _typeDef.BaseType);
		_interfaces = _typeDef.Interfaces.Select(i => new InterfaceTypeUri(i.InterfaceType.Scope.GetAssemblyName(), i.InterfaceType.FullName)).ToArray();

		IsBaseInitialized = true;
	}

	void IDeferredTypeModel.FullyInitialize()
	{
		if (IsFullyInitialized)
		{
			return;
		}

		ScanUFunctions();
		ScanUProperties();
		
		IsFullyInitialized = true;
	}

	public override IReadOnlyCollection<IUnrealReflectionSpecifier> Specifiers
	{
		get
		{
			check(IsFullyInitialized);
			return base.Specifiers;
		}
	}

	public override IReadOnlyList<IUnrealPropertyModel> Properties
	{
		get
		{
			check(IsFullyInitialized);
			return base.Properties;
		}
	}

	public IModelRegistry Registry => _registry;
	public string AssemblyName { get; }
	public string FullName { get; }

	public TypeModelReference? BaseType
	{
		get
		{
			check(IsBaseInitialized);

			return _baseType;
		}
	}
	
	public TypeModelReference EnumUnderlyingType => throw new NotSupportedException();

	public IReadOnlyList<InterfaceTypeUri> Interfaces
	{
		get
		{
			check(IsBaseInitialized);

			return _interfaces;
		}
	}
	
	public IReadOnlyList<IUnrealFunctionModel> Functions => _functions;
	public bool HasUClassConstructor { get; }
	public bool IsInternal { get; }

	public bool IsBaseInitialized { get; private set; }
	public bool IsFullyInitialized { get; private set; }
	
	private void ScanUFunctions()
	{
		foreach (var method in _typeDef.Methods)
		{
			if (!IsUFunction(method, out var eventOverrideName))
			{
				continue;
			}
			
			_functions.Add(new(_registry, method, this, eventOverrideName));
		}
	}

	private void ScanUProperties()
	{
		foreach (var property in _typeDef.Properties.Where(CustomAttributeProviderExtensions.HasCustomAttribute<UPropertyAttribute>))
		{
			_properties.Add(new(_registry, property, this));
		}
	}

	private bool IsUFunction(MethodDefinition method, out string? eventOverrideName)
	{
		eventOverrideName = null;
		
		// Method marked as UFunction is a UFunction.
		if (method.HasCustomAttribute<UFunctionAttribute>())
		{
			return true;
		}

		// The FIRST override method ends with _Implementation with root marked as Event is a UFunction event override.
		if (method is { IsVirtual: true, IsReuseSlot: true } && method.Name.EndsWith("_Implementation"))
		{
			TypeDefinition? currentType = _typeDef;
			currentType = currentType.BaseType is not null ? _registry.ResolveTypeDefinition(currentType.BaseType) : null;
			while (currentType is not null)
			{
				foreach (var methodToTest in currentType.Methods)
				{
					// Sealed function is not our target.
					if (!methodToTest.IsVirtual)
					{
						continue;
					}
					
					// Signature mismatch.
					if (!IsSignatureMatch(method, methodToTest))
					{
						continue;
					}
					
					// We have parent override version so we are not that UFunction.
					if (!methodToTest.IsNewSlot)
					{
						return false;
					}

					// Now this is our root virtual method and let's see if it is marked as Event.
					foreach (var attribute in methodToTest.CustomAttributes)
					{
						if (attribute.AttributeType.FullName == typeof(EventImplementationAttribute).FullName)
						{
							eventOverrideName = (string)attribute.ConstructorArguments[0].Value;
							return true;
						}
					}

					return false;
				}

				currentType = currentType.BaseType is not null ? _registry.ResolveTypeDefinition(currentType.BaseType) : null;
			}
		}

		// Otherwise not a UFunction.
		return false;
	}

	private bool IsSignatureMatch(MethodDefinition lhs, MethodDefinition rhs)
	{
		if (lhs.Name != rhs.Name)
		{
			return false;
		}

		if (lhs.Parameters.Count != rhs.Parameters.Count)
		{
			return false;
		}

		for (int32 i = 0; i < lhs.Parameters.Count; ++i)
		{
			ParameterDefinition lhsParameter = lhs.Parameters[i];
			ParameterDefinition rhsParameter = rhs.Parameters[i];
			if (lhsParameter.ParameterType.FullName != rhsParameter.ParameterType.FullName)
			{
				return false;
			}
		}

		return true;
	}

	private readonly ModelRegistry _registry;
	
	private readonly TypeDefinition _typeDef;

	private TypeModelReference? _baseType;
	private InterfaceTypeUri[] _interfaces = null!;

	private readonly List<UnrealFunctionModel> _functions = new();
	
}


