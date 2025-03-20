// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using System.Runtime.Loader;

namespace ZeroGames.ZSharp.Core;

public interface IAssemblyResolver
{
	static IAssemblyResolver Create()
	{
		AssemblyResolverAttribute attribute = AssemblyLoadContext.Default.Assemblies
			.Select(asm => asm.GetCustomAttribute<AssemblyResolverAttribute>())
			.Single(attr => attr is not null)!;
		Type resolverType = attribute.ResolverType;
		if (!resolverType.IsAssignableTo(typeof(IAssemblyResolver)))
		{
			throw new InvalidOperationException();
		}

		var resolver = Activator.CreateInstance(resolverType) as IAssemblyResolver;
		if (resolver is null)
		{
			throw new InvalidOperationException();
		}
		
		return resolver;
	}
	
	string? Resolve(string name);
}


