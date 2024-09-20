// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.Loader;
using System.Text.Json;
using System.Threading.Tasks;
using Mono.Cecil;
using IAssemblyResolver = ZeroGames.ZSharp.Core.IAssemblyResolver;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

internal partial class UnrealFieldScanner : IDisposable
{
	
	public UnrealFieldScanner(string assemblyName, string moduleName, bool withMetadata)
	{
		Assembly resolverAssembly = AssemblyLoadContext.Default.Assemblies.Single(asm => asm.GetCustomAttribute<AssemblyResolverAttribute>() is not null);
		Type resolverType = resolverAssembly.GetCustomAttribute<AssemblyResolverAttribute>()!.ResolverType;
		if (!resolverType.IsAssignableTo(typeof(IAssemblyResolver)))
		{
			throw new InvalidOperationException();
		}

		_assemblyName = assemblyName;
		_moduleName = moduleName;
		_withMetadata = withMetadata;
		
		_resolver = Unsafe.As<IAssemblyResolver>(Activator.CreateInstance(resolverType)!);
		if (_resolver.Resolve(assemblyName) is { } dllPath)
		{
			_assembly = LoadAssemblyDefinition(dllPath);
		}

		_manifest = new() { ModuleName = _moduleName };
	}

	~UnrealFieldScanner()
	{
		InternalDispose();
	}
	
	public void Dispose()
	{
		InternalDispose();
		GC.SuppressFinalize(this);
	}

	public string Scan()
	{
		//BlockUntilDebuggerAttached();
		
		if (_assembly is null)
		{
			return string.Empty;
		}
		
		if (_result is not null)
		{
			return _result;
		}

		Parallel.ForEachAsync(_assembly.Modules.SelectMany(module => module.Types), (type, _) => ScanTypeDefinition(type)).GetAwaiter().GetResult();

		lock (_manifest)
		{
			_result = _manifest.IsEmpty ? string.Empty : JsonSerializer.Serialize(_manifest);
		}
		return _result;
	}
	
	private class PoisonedAssemblyResolver : Mono.Cecil.IAssemblyResolver
	{
		public void Dispose(){}
		public AssemblyDefinition Resolve(AssemblyNameReference name) => throw new NotSupportedException();
		public AssemblyDefinition Resolve(AssemblyNameReference name, ReaderParameters parameters) => throw new NotSupportedException();
	}
	
	private ValueTask ScanTypeDefinition(TypeDefinition type)
	{
		string typeFullName = type.FullName;
		string[] names = typeFullName.Split('.');
		// Valid class path name of unreal field requires AssemblyName.ModuleName.FieldName
		// so name with less than 3 segments must be invalid.
		if (names.Length < 3)
		{
			return ValueTask.CompletedTask;
		}

		// ModuleName mismatch.
		if (names[^2] != _moduleName)
		{
			return ValueTask.CompletedTask;
		}
		
		// AssemblyName mismatch.
		string assemblyNameToTest = string.Join('.', names.Take(names.Length - 2));
		if (assemblyNameToTest != _assemblyName)
		{
			return ValueTask.CompletedTask;
		}
		
		if (GetCustomAttributeOrDefault<UClassAttribute>(type) is {} uclassAttr)
		{
			ScanUClass(type, uclassAttr);
		}

		return ValueTask.CompletedTask;
	}

	// We don't use Mono.Cecil's built-in assembly resolving mechanism
	// since it doesn't have transitivity and can make things more difficult.
	private AssemblyDefinition? GetAssemblyDefinition(string name)
	{
		if (name == _assembly?.Name.Name)
		{
			return _assembly;
		}
		
		lock (_externalAssemblyMap)
		{
			if (!_externalAssemblyMap.TryGetValue(name, out var assembly))
			{
				if (_resolver.Resolve(name) is { } dllPath)
				{
					assembly = LoadAssemblyDefinition(dllPath);
					_externalAssemblyMap[name] = assembly;
				}
			}

			return assembly;
		}
	}

	private AssemblyDefinition LoadAssemblyDefinition(string dllPath) => AssemblyDefinition.ReadAssembly(dllPath, new() { AssemblyResolver = new PoisonedAssemblyResolver() });
	
	private void InternalDispose()
	{
		if (_disposed)
		{
			return;
		}

		_disposed = true;
		
		_assembly?.Dispose();
		
		foreach (var pair in _externalAssemblyMap)
		{
			pair.Value.Dispose();
		}
	}

	private readonly string _assemblyName;
	private readonly string _moduleName;
	private readonly bool _withMetadata;
	
	private IAssemblyResolver _resolver;
	
	private AssemblyDefinition? _assembly;
	private Dictionary<string, AssemblyDefinition> _externalAssemblyMap = new();

	private UnrealFieldManifest _manifest;
	private string? _result;

	private bool _disposed;

}


