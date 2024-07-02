// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public partial class GameplayStatics
{

	public static unsafe Character? GetPlayerCharacter(UnrealObject? worldContextObject, int32 playerIndex)
	{
		IMasterAssemblyLoadContext alc = GetOwningAlc();
		const int32 numSlots = 3;
		ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[numSlots]
		{
			ZCallBufferSlot.FromConjugate(worldContextObject),
			ZCallBufferSlot.FromInt32(playerIndex),
			ZCallBufferSlot.FromConjugate(null),
		};
		ZCallBuffer buffer = new(slots, numSlots);
		ZCallHandle handle = alc.GetZCallHandle("uf://Script/Engine.GameplayStatics:GetPlayerCharacter");
		alc.ZCall(handle, &buffer);

		return slots[2].ReadConjugate<Character>();
	}
	
}