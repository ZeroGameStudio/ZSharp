// Copyright Zero Games. All Rights Reserved.

using Mono.Cecil;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

internal class UnrealDelegateModel : UnrealStructModel, IUnrealDelegateModel, IDeferredTypeModel
{

	public UnrealDelegateModel(ModelRegistry registry, TypeDefinition typeDef, IUnrealClassModel? outer = null) : base(typeDef.Name.Substring(1) + "__DelegateSignature", registry, typeDef)
	{
		AssemblyName = typeDef.Scope.GetAssemblyName();
		FullName = typeDef.FullName;

		_registry = registry;
		_typeDef = typeDef;
		
		Outer = outer;
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

		ScanUParams(_registry, _typeDef.NestedTypes.Single(t => t.Name is "Signature" && !t.HasGenericParameters).Methods.Single(m => m.Name is "Invoke"));
		
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
	
	public IUnrealClassModel? Outer { get; }

	private void ScanUParams(ModelRegistry registry, MethodDefinition methodDef)
	{
		foreach (var parameter in methodDef.Parameters)
		{
			_properties.Add(new(registry, parameter, this));
		}

		if (methodDef.ReturnType.FullName != typeof(void).FullName)
		{
			_properties.Add(new(registry, methodDef.MethodReturnType, this));
		}
	}
	
	private readonly ModelRegistry _registry;
	
	private readonly TypeDefinition _typeDef;

	private TypeModelReference? _baseType;
	private InterfaceTypeUri[] _interfaces = null!;
	
}