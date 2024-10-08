// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.Loader;

namespace ZeroGames.ZSharp.Core;

public interface IAssemblyResolver
{
	static IAssemblyResolver Create()
	{
		Assembly resolverAssembly = AssemblyLoadContext.Default.Assemblies.Single(asm => asm.GetCustomAttribute<AssemblyResolverAttribute>() is not null);
		Type resolverType = resolverAssembly.GetCustomAttribute<AssemblyResolverAttribute>()!.ResolverType;
		if (!resolverType.IsAssignableTo(typeof(IAssemblyResolver)))
		{
			throw new InvalidOperationException();
		}
		
		return Unsafe.As<IAssemblyResolver>(Activator.CreateInstance(resolverType)!);
	}
	
	string? Resolve(string name);
}


