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
	}

	public void BaseInitialize()
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

	public void FullyInitialize()
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
			CheckInvariant();
			return base.Specifiers;
		}
	}

	public override IReadOnlyList<IUnrealPropertyModel> Properties
	{
		get
		{
			CheckInvariant();
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
			if (!IsBaseInitialized)
			{
				throw new InvalidOperationException();
			}

			return _baseType;
		}
	}

	public IReadOnlyList<InterfaceTypeUri> Interfaces
	{
		get
		{
			if (!IsBaseInitialized)
			{
				throw new InvalidOperationException();
			}

			return _interfaces;
		}
	}
	
	public IReadOnlyList<IUnrealFunctionModel> Functions => _functions;

	public bool IsBaseInitialized { get; private set; }
	public bool IsFullyInitialized { get; private set; }

	private void CheckInvariant()
	{
		if (!IsFullyInitialized)
		{
			throw new InvalidOperationException();
		}
	}
	
	private void ScanUFunctions()
	{
		foreach (var method in _typeDef.Methods.Where(CustomAttributeProviderExtensions.HasCustomAttribute<UFunctionAttribute>))
		{
			_functions.Add(new(_registry, method, this));
		}
	}

	private void ScanUProperties()
	{
		foreach (var property in _typeDef.Properties.Where(CustomAttributeProviderExtensions.HasCustomAttribute<UPropertyAttribute>))
		{
			_properties.Add(new(_registry, property, this));
		}
	}

	private readonly ModelRegistry _registry;
	
	private readonly TypeDefinition _typeDef;

	private TypeModelReference? _baseType;
	private InterfaceTypeUri[] _interfaces = null!;

	private readonly List<UnrealFunctionModel> _functions = new();
	
}


