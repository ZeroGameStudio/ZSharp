// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

public unsafe interface IMasterAssemblyLoadContext : IZSharpAssemblyLoadContext
{
	
	public static IMasterAssemblyLoadContext? Get() => MasterAssemblyLoadContext.Get();

	int32 ZCall(ZCallHandle handle, ZCallBuffer* buffer);
	int32 ZCall(string name, ZCallBuffer* buffer);
	int32 ZCall(string name, ZCallBuffer* buffer, out ZCallHandle handle);
	void ZCall_AnyThread(ZCallHandle handle, ZCallBuffer* buffer, int32 numSlots);
	ZCallHandle GetZCallHandle(string name);

}


