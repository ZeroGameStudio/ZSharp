// Copyright Zero Games. All Rights Reserved.

using Mono.Cecil;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

internal class UnrealClassModel : UnrealStructModel, IUnrealClassModel, IDeferredTypeModel
{

	public UnrealClassModel(ModelRegistry registry, TypeDefinition typeDef) : base(typeDef.Name, registry, typeDef)
	{
		_registry = registry;
		_typeDef = typeDef;
		
		FullName = typeDef.FullName;
	}

	public void BaseInitialize()
	{
		if (IsBaseInitialized)
		{
			return;
		}
		
		_baseType = new((ModelRegistry)Registry, _typeDef.BaseType);

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
	public string FullName { get; }

	public TypeModelReference BaseType
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
			_functions.Add(new(_registry, method));
		}
	}

	private void ScanUProperties()
	{
		foreach (var property in _typeDef.Properties.Where(CustomAttributeProviderExtensions.HasCustomAttribute<UPropertyAttribute>))
		{
			_properties.Add(new(_registry, property));
		}
	}

	private readonly ModelRegistry _registry;
	
	private readonly TypeDefinition _typeDef;

	private TypeModelReference _baseType;
	
	private readonly List<UnrealFunctionModel> _functions = new();
	
}


