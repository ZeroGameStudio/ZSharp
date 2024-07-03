// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public partial class CharacterMovementComponent
{
	
	public unsafe float MaxWalkSpeed
	{
		get
		{
			IMasterAssemblyLoadContext alc = GetOwningAlc();
			const int32 numSlots = 3;
			ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[numSlots]
			{
				ZCallBufferSlot.FromConjugate(this),
				ZCallBufferSlot.FromBool(false),
				ZCallBufferSlot.FromFloat(0.0f),
			};
			ZCallBuffer buffer = new(slots, numSlots);
			ZCallHandle handle = alc.GetZCallHandle("up://Script/Engine.CharacterMovementComponent:MaxWalkSpeed");
			alc.ZCall(handle, &buffer);

			return slots[2].ReadFloat();
		}
		set
		{
			IMasterAssemblyLoadContext alc = GetOwningAlc();
			const int32 numSlots = 3;
			ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[numSlots]
			{
				ZCallBufferSlot.FromConjugate(this),
				ZCallBufferSlot.FromBool(true),
				ZCallBufferSlot.FromFloat(value),
			};
			ZCallBuffer buffer = new(slots, numSlots);
			ZCallHandle handle = alc.GetZCallHandle("up://Script/Engine.CharacterMovementComponent:MaxWalkSpeed");
			alc.ZCall(handle, &buffer);
		}
	}
	
}