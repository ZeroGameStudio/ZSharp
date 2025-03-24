// Copyright Zero Games. All Rights Reserved.

using Mono.Cecil;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

internal class UnrealScriptStructModel : UnrealStructModel, IUnrealScriptStructModel, IDeferredTypeModel
{
	
	public UnrealScriptStructModel(ModelRegistry registry, TypeDefinition typeDef) : base(typeDef.Name)
	{
		_registry = registry;
		_typeDef = typeDef;

		AssemblyName = typeDef.Scope.GetAssemblyName();
		FullName = typeDef.FullName;
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

	public string AssemblyName { get; }
	public string FullName { get; }

	public IModelRegistry Registry => _registry;
	
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
	
	public bool IsBaseInitialized { get; private set; }
	public bool IsFullyInitialized { get; private set; }
	
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
	
}


