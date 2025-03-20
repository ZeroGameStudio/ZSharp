// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;
using System.Threading;
using System.Threading.Tasks;
using Mono.Cecil;
using IAssemblyResolver = ZeroGames.ZSharp.Core.IAssemblyResolver;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

internal sealed partial class ModelRegistry : IModelRegistry
{

	public ModelRegistry(string rootAssemblyName, string rootModuleName, bool withMetadata)
	{
		// BlockUntilDebuggerAttached();
		
		RootAssemblyName = rootAssemblyName;
		RootModuleName = rootModuleName;
		WithMetadata = withMetadata;
		
		Setup();

		IsFullyInitialized = true;
	}

	public ITypeModel GetTypeModel(string fullName)
	{
		check(IsFullyInitialized);
		return GetTypeModelUnchecked(fullName);
	}

	public bool TryGetTypeModel(string fullName, [NotNullWhen(true)] out ITypeModel? typeModel)
	{
		check(IsFullyInitialized);
		return TryGetTypeModelUnchecked(fullName, out typeModel);
	}
	
	
	public ITypeModel GetTypeModelUnchecked(string fullName) => _typeMap[fullName];
	public bool TryGetTypeModelUnchecked(string fullName, [NotNullWhen(true)] out ITypeModel? typeModel) => _typeMap.TryGetValue(fullName, out typeModel);
	
	public ITypeModel GetOrAddTypeModel(TypeReference typeRef)
	{
		check(!IsFullyInitialized);
		
		if (!TryGetTypeModelUnchecked(typeRef.FullName, out var model))
		{
			check(!typeRef.IsArray);
			
			TypeDefinition typeDef = GetTypeDefinition(typeRef);
			model = new PlainTypeModel(this, typeDef);
			_typeMap[model.FullName] = model;
		}

		return model;
	}

	public TypeDefinition ResolveTypeDefinition(string assemblyName, string typeFullName)
	{
		check(!IsFullyInitialized);
		return GetTypeDefinition(assemblyName, typeFullName);
	}

	public TypeDefinition ResolveTypeDefinition(TypeReference typeRef) => ResolveTypeDefinition(typeRef.Scope.GetAssemblyName(), typeRef.FullName);
	
	public string RootAssemblyName { get; }
	public string RootModuleName { get; }
	public bool WithMetadata { get; }

	public IEnumerable<ITypeModel> RootTypes => _rootTypes;
	
	public bool IsFullyInitialized { get; }

	private class AssemblyContainer : IDisposable
	{
		public void Dispose()
		{
			InternalDispose();
			GC.SuppressFinalize(this);
		}

		public bool TryGetValue(string assemblyName, [NotNullWhen(true)] out AssemblyDefinition? assemblyDefinition) => _assemblyMap.TryGetValue(assemblyName, out assemblyDefinition);
		public void Add(AssemblyDefinition assembly) => _assemblyMap.Add(assembly.Name.Name, assembly);

		~AssemblyContainer()
		{
			InternalDispose();
		}
		
		private void InternalDispose()
		{
			foreach (var pair in _assemblyMap)
			{
				pair.Value.Dispose();
			}
			
			_assemblyMap.Clear();
		}
		
		private Dictionary<string, AssemblyDefinition> _assemblyMap = new();
	}

	private void Setup()
	{
		SetupIntrinsicTypes();
		
		AssemblyDefinition rootAssembly = LoadAssemblyDefinition(RootAssemblyName);
		
		// First generate skeleton for root types. This step can run in parallel.
		Parallel.ForEach(rootAssembly.Modules.SelectMany(module => module.Types), (type, _) => ScanTypeDefinition(type));
		
		// Then initialize base type for root types. Because root type may have root type base so use an individual step to initialize.
		foreach (var rootType in _rootTypes)
		{
			if (rootType is IDeferredTypeModel deferredRootType)
			{
				deferredRootType.BaseInitialize();
			}
		}
		
		// Then fully initialize root types, load all reachable types and setup dependency graph. This step must run in serial.
		foreach (var rootType in _rootTypes)
		{
			if (rootType is IDeferredTypeModel deferredRootType)
			{
				deferredRootType.FullyInitialize();
			}
		}
	}
	
	private void ScanTypeDefinition(TypeDefinition typeDef)
	{
		string typeFullName = typeDef.FullName;
		string[] names = typeFullName.Split('.');
		// Type without namespace is invalid.
		if (names.Length < 2)
		{
			return;
		}

		// ModuleName mismatch.
		if (names[^2] != RootModuleName)
		{
			return;
		}
		
		// AssemblyName mismatch.
		string maybeAssemblyName1 = string.Join('.', names.Take(names.Length - 2)); // AssemblyName.ModuleName.FieldName
		string maybeAssemblyName2 =string.Join('.', names.Take(names.Length - 1)); // AssemblyName.FieldName treats last segment of AssemblyName as ModuleName
		if (maybeAssemblyName1 != RootAssemblyName && maybeAssemblyName2 != RootAssemblyName)
		{
			return;
		}

		List<ITypeModel> models = [];
		if (typeDef.HasCustomAttribute<UEnumAttribute>())
		{
			models.Add(new UnrealEnumModel(this, typeDef));
		}
		else if (typeDef.HasCustomAttribute<UStructAttribute>())
		{
			models.Add(new UnrealScriptStructModel(this, typeDef));
		}
		else if (typeDef.HasCustomAttribute<UDelegateAttribute>())
		{
			models.Add(new UnrealDelegateModel(this, typeDef));
		}
		else if (typeDef.HasCustomAttribute<UClassAttribute>())
		{
			UnrealClassModel model = new(this, typeDef);
			models.Add(model);
			// UClass may have nested UDelegate in it.
			foreach (var delegateTypeDef in typeDef.NestedTypes.Where(t => t.HasCustomAttribute<UDelegateAttribute>()))
			{
				models.Add(new UnrealDelegateModel(this, delegateTypeDef, model));
			}
		}

		foreach (var model in models)
		{
			lock (_containerLock)
			{
				_rootTypes.Add(model);
				_typeMap[model.FullName] = model;
			}
		}
	}
	
	private readonly IAssemblyResolver _resolver = IAssemblyResolver.Create();

	private readonly Lock _containerLock = new();
	private readonly List<ITypeModel> _rootTypes = new();
	private readonly Dictionary<string, ITypeModel> _typeMap = new();
	
}


