// Copyright Zero Games. All Rights Reserved.

using System.Reflection;

namespace ZeroGames.ZSharp.Core;

public interface IZSharpAssemblyLoadContext
{
	unsafe ELoadAssemblyErrorCode LoadAssembly(string name, void* args, out Assembly? assembly);
	unsafe EInvokeMethodErrorCode InvokeMethod(string assemblyName, string typeName, string methodName, void* args);
	bool IsUnloaded { get; }
}

public static class ZSharpAssemblyLoadContextExtensions
{

	public static void GuardUnloaded(this IZSharpAssemblyLoadContext @this)
	{
		if (@this.IsUnloaded)
		{
			throw new AlcUnloadedException();
		}
	}
	
}


