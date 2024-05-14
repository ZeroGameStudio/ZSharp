// Copyright Zero Games. All Rights Reserved.

using System.Reflection;

namespace ZeroGames.ZSharp.Core;

public unsafe interface IMasterAssemblyLoadContext : IZSharpAssemblyLoadContext
{
	
	public static IMasterAssemblyLoadContext? Get() => MasterAssemblyLoadContext.Get();

	Assembly? LoadAssembly(string name);
	Type? GetType(string assemblyName, string typeName);

	ZCallHandle RegisterZCall(IZCallDispatcher dispatcher);
	void RegisterZCallResolver(IZCallResolver resolver, uint64 priority);
	
	int32 ZCall(ZCallHandle handle, ZCallBuffer* buffer);
	ZCallHandle GetZCallHandle(string name);
	IntPtr BuildConjugate(uint16 registryId, IConjugate managed);
	void ReleaseConjugate(uint16 registryId, IntPtr unmanaged);
	void PushPendingDisposeConjugate(IConjugate conjugate);
}


