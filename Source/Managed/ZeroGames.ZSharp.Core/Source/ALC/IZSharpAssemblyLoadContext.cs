// Copyright Zero Games. All Rights Reserved.

using System.Reflection;

namespace ZeroGames.ZSharp.Core;

public interface IZSharpAssemblyLoadContext : IGCHandle
{
	unsafe ELoadAssemblyErrorCode LoadAssembly(string name, void* args, out Assembly? assembly);
	unsafe EInvokeMethodErrorCode InvokeMethod(string assemblyName, string typeName, string methodName, void* args);
	bool IsUnloaded { get; }
}


