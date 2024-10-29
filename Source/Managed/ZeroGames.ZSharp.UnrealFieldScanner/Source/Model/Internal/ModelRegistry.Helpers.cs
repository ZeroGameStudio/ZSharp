// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;
using System.Threading;
using Mono.Cecil;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

partial class ModelRegistry
{
	
	private sealed class AssemblyResolver(ModelRegistry registry) : Mono.Cecil.IAssemblyResolver
	{
		public void Dispose(){}
		public AssemblyDefinition Resolve(AssemblyNameReference name) => Resolve(name, new() { AssemblyResolver = new AssemblyResolver(_registry) });
		public AssemblyDefinition Resolve(AssemblyNameReference name, ReaderParameters parameters) => _registry.LoadAssemblyDefinition(name.Name);
		private ModelRegistry _registry = registry;
	}

	private AssemblyDefinition LoadAssemblyDefinition(string assemblyName)
	{
		lock (_referencedAssemblyMap)
		{
			if (!_referencedAssemblyMap.TryGetValue(assemblyName, out var assembly))
			{
				assembly = AssemblyDefinition.ReadAssembly(_resolver.Resolve(assemblyName), new() { AssemblyResolver = new AssemblyResolver(this) });
				_referencedAssemblyMap.Add(assembly);
			}

			return assembly;
		}
	}

	private TypeDefinition GetTypeDefinition(TypeReference typeRef) => GetTypeDefinition(typeRef.Scope.GetAssemblyName(), typeRef.FullName);
	private TypeDefinition GetTypeDefinition(string assemblyName, string typeFullName)
	{
		AssemblyDefinition assembly = LoadAssemblyDefinition(assemblyName);
		return assembly.Modules.SelectMany(module => module.Types).Single(t => t.FullName == typeFullName);
	}

	[Conditional("DEBUG")]
	private void BlockUntilDebuggerAttached()
	{
		while (!Debugger.IsAttached)
		{
			Thread.Sleep(1000);
			UE_WARNING(LogTemp, "Waiting for debugger...");
		}
		Debugger.Break();
	}

}


