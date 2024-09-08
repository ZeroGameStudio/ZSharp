// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.Loader;
using Mono.Cecil;
using IAssemblyResolver = ZeroGames.ZSharp.Core.IAssemblyResolver;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

internal class UnrealFieldScanner
{

	public static UnrealFieldScanner Instance { get; } = new();

	public string Scan(string assemblyName)
	{
		string? dllPath = _resolver.Resolve(assemblyName);
		if (dllPath is null)
		{
			return string.Empty;
		}
		
		using AssemblyDefinition assemblyDef = AssemblyDefinition.ReadAssembly(dllPath);

		// @TODO
		return assemblyDef.Name.ToString();
	}

	private UnrealFieldScanner()
	{
		Assembly resolverAssembly = AssemblyLoadContext.Default.Assemblies.Single(asm => asm.GetCustomAttribute<AssemblyResolverAttribute>() is not null);
		Type resolverType = resolverAssembly.GetCustomAttribute<AssemblyResolverAttribute>()!.ResolverType;
		if (!resolverType.IsAssignableTo(typeof(IAssemblyResolver)))
		{
			throw new InvalidOperationException();
		}
        
		_resolver = Unsafe.As<IAssemblyResolver>(Activator.CreateInstance(resolverType)!);
	}
	
	private IAssemblyResolver _resolver;

}


