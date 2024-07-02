// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public partial class Character
{

	public unsafe void Jump()
	{
		IMasterAssemblyLoadContext alc = GetOwningAlc();
		const int32 numSlots = 1;
		ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[numSlots]
		{
			ZCallBufferSlot.FromConjugate(this),
		};
		ZCallBuffer buffer = new(slots, numSlots);
		ZCallHandle handle = alc.GetZCallHandle("uf://Script/Engine.Character:Jump");
		alc.ZCall(handle, &buffer);
	}
	
}