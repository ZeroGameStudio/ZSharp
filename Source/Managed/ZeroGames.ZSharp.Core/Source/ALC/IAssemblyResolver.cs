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
		
		check(resolverType.IsAssignableTo(typeof(IAssemblyResolver)));
		verify(Activator.CreateInstance(resolverType) is var resolver && resolver is not null);
		return (IAssemblyResolver)resolver;
	}
	
	string? Resolve(string name);
}


