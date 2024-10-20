// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using System.Runtime.Loader;

namespace ZeroGames.ZSharp.Core;

internal class DefaultAssemblyLoadContextProxy
{

	public static DefaultAssemblyLoadContextProxy Instance { get; } = new();

	public unsafe ELoadAssemblyErrorCode LoadAssembly(string name, void* args, out Assembly? assembly)
	{
		return AssemblyLoadContextHelper.LoadAssembly(AssemblyLoadContext.Default, _resolver, false, name, args, out assembly);
	}

	public unsafe EInvokeMethodErrorCode InvokeMethod(string assemblyName, string typeName, string methodName, void* args)
	{
		return AssemblyLoadContextHelper.InvokeMethod(AssemblyLoadContext.Default, assemblyName, typeName, methodName, args);
	}

	private DefaultAssemblyLoadContextProxy()
	{
		_resolver = IAssemblyResolver.Create();
	}

	private IAssemblyResolver _resolver;

}


