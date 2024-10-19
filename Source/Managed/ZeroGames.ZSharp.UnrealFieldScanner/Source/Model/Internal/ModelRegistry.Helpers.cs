// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;
using System.Threading;
using Mono.Cecil;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

partial class ModelRegistry
{
	
	private class PoisonedAssemblyResolver : Mono.Cecil.IAssemblyResolver
	{
		public void Dispose(){}
		public AssemblyDefinition Resolve(AssemblyNameReference name) => throw new NotSupportedException();
		public AssemblyDefinition Resolve(AssemblyNameReference name, ReaderParameters parameters) => throw new NotSupportedException();
	}

	private void CheckDuringInitialization()
	{
		if (IsFullyInitialized)
		{
			throw new InvalidOperationException();
		}
	}

	private void CheckInvariant()
	{
		if (!IsFullyInitialized)
		{
			throw new InvalidOperationException();
		}
	}
	
	private AssemblyDefinition LoadAssemblyDefinition(string assemblyName)
	{
		lock (_referencedAssemblyMap)
		{
			if (!_referencedAssemblyMap.TryGetValue(assemblyName, out var assembly))
			{
				assembly = AssemblyDefinition.ReadAssembly(_resolver.Resolve(assemblyName), new() { AssemblyResolver = new PoisonedAssemblyResolver() });
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

	private void BlockUntilDebuggerAttached()
	{
		while (!Debugger.IsAttached)
		{
			Thread.Sleep(1000);
			UE_WARNING("Waiting for debugger...");
		}
		Debugger.Break();
	}

}


