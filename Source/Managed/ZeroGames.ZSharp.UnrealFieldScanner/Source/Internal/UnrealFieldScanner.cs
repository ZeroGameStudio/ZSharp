// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.Loader;
using System.Text.Json;
using System.Threading.Tasks;
using Mono.Cecil;
using IAssemblyResolver = ZeroGames.ZSharp.Core.IAssemblyResolver;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

internal class UnrealFieldScanner : IDisposable
{
	
	public UnrealFieldScanner(string assemblyName, bool withMetadata)
	{
		Assembly resolverAssembly = AssemblyLoadContext.Default.Assemblies.Single(asm => asm.GetCustomAttribute<AssemblyResolverAttribute>() is not null);
		Type resolverType = resolverAssembly.GetCustomAttribute<AssemblyResolverAttribute>()!.ResolverType;
		if (!resolverType.IsAssignableTo(typeof(IAssemblyResolver)))
		{
			throw new InvalidOperationException();
		}

		_withMetadata = withMetadata;
		_resolver = Unsafe.As<IAssemblyResolver>(Activator.CreateInstance(resolverType)!);
		if (_resolver.Resolve(assemblyName) is { } dllPath)
		{
			_assembly = LoadAssemblyDefinition(dllPath);
		}

		_def = new() { Path = ResolvePackagePath(assemblyName) };
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
		// while (!Debugger.IsAttached)
		// {
		// 	Thread.Sleep(1000);
		// 	Logger.Warning("Waiting for debugger...");
		// }
		// Debugger.Break();
		
		if (_assembly is null)
		{
			return string.Empty;
		}
		
		if (_result is not null)
		{
			return _result;
		}

		Parallel.ForEachAsync(_assembly.Modules.SelectMany(module => module.Types), (type, _) => ScanTypeDefinition(type)).GetAwaiter().GetResult();

		lock (_def)
		{
			_result = _def.IsEmpty ? string.Empty : JsonSerializer.Serialize(_def);
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
		string uclassAttributeFullName = typeof(UClassAttribute).FullName!;
		foreach (var attr in type.CustomAttributes)
		{
			if (attr.AttributeType.FullName == uclassAttributeFullName)
			{
				ScanUClass(type);
				break;
			}
		}

		return ValueTask.CompletedTask;
	}

	private void ScanUClass(TypeDefinition type)
	{
		string name = type.Name;
		EObjectFlags flags = EObjectFlags.RF_NoFlags;
		EClassFlags classFlags = EClassFlags.CLASS_None;
		EClassCastFlags castFlags = EClassCastFlags.CASTCLASS_None;
		UnrealClassDefinition cls = new()
		{
			Name = name,
			Flags = flags,
			ClassFlags = classFlags,
			CastFlags = castFlags
		};

		string baseName = type.BaseType.Name;
		IMetadataScope scope = type.BaseType.Scope;
		string baseAssemblyName = scope.MetadataScopeType == MetadataScopeType.AssemblyNameReference ? scope.Name : throw new InvalidOperationException();
		AssemblyDefinition baseAssembly = GetAssemblyDefinition(baseAssemblyName) ?? throw new InvalidOperationException();
		TypeDefinition baseType = baseAssembly.Modules.SelectMany(module => module.Types).Single(t => t.Name == baseName);
		foreach (var attr in baseType.CustomAttributes)
		{
			if (attr.AttributeType.FullName == UNREAL_FIELD_PATH_ATTRIBUTE_FULL_NAME)
			{
				cls.SuperPath = (string)attr.ConstructorArguments[0].Value;
				break;
			}
		}

		if (_withMetadata)
		{
			cls.MetadataMap["IsBlueprintBase"] = "true";
			cls.MetadataMap["BlueprintType"] = "true";
		}
		
		lock (_def.ClassMap)
		{
			_def.ClassMap[cls.Name] = cls;
		}
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

	private string ResolvePackagePath(string assemblyName)
	{
		string subpath = assemblyName.Replace('.', '/');
		return $"/Script/ZSharpEmit/{subpath}";
	}
	
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

	private const string UNREAL_FIELD_PATH_ATTRIBUTE_FULL_NAME = "ZeroGames.ZSharp.UnrealEngine.UnrealFieldPathAttribute";

	private readonly bool _withMetadata;
	
	private IAssemblyResolver _resolver;
	
	private AssemblyDefinition? _assembly;
	private Dictionary<string, AssemblyDefinition> _externalAssemblyMap = new();

	private UnrealPackageDefinition _def;
	private string? _result;

	private bool _disposed;

}


