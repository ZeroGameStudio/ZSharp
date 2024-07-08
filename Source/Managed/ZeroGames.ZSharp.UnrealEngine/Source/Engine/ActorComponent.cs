// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public partial class ActorComponent
{

	public unsafe Actor? Owner
	{
		get
		{
			IMasterAssemblyLoadContext alc = GetOwningAlc();
			const int32 numSlots = 2;
			ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[numSlots]
			{
				ZCallBufferSlot.FromConjugate(this),
				ZCallBufferSlot.FromConjugate(),
			};
			ZCallBuffer buffer = new(slots, numSlots);
			ZCallHandle handle = alc.GetZCallHandle("uf://Script/Engine.ActorComponent:GetOwner");
			alc.ZCall(handle, &buffer);

			return slots[1].ReadConjugate<Actor>();
		}
	}
	
}


