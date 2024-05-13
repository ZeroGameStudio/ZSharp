// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

public unsafe interface IMasterAssemblyLoadContext : IZSharpAssemblyLoadContext
{
	
	public static IMasterAssemblyLoadContext? Get() => MasterAssemblyLoadContext.Get();

	int32 ZCall(ZCallHandle handle, ZCallBuffer* buffer);
	ZCallHandle GetZCallHandle(string name);
	IntPtr BuildConjugate(uint16 registryId, IConjugate managed);
	void ReleaseConjugate(uint16 registryId, IntPtr unmanaged);

}


