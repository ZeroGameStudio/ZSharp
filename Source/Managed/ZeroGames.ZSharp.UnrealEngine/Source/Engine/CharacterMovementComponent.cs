// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public partial class CharacterMovementComponent
{
	
	public unsafe float MaxWalkSpeed
	{
		get
		{
			IMasterAssemblyLoadContext alc = GetOwningAlc();
			const int32 numSlots = 4;
			ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[numSlots]
			{
				ZCallBufferSlot.FromConjugate(this),
				ZCallBufferSlot.FromBool(),
				ZCallBufferSlot.FromInt32(),
				ZCallBufferSlot.FromFloat(),
			};
			ZCallBuffer buffer = new(slots, numSlots);
			ZCallHandle handle = alc.GetZCallHandle("up://Script/Engine.CharacterMovementComponent:MaxWalkSpeed");
			alc.ZCall(handle, &buffer);

			return slots[3].Float;
		}
		set
		{
			IMasterAssemblyLoadContext alc = GetOwningAlc();
			const int32 numSlots = 4;
			ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[numSlots]
			{
				ZCallBufferSlot.FromConjugate(this),
				ZCallBufferSlot.FromBool(true),
				ZCallBufferSlot.FromInt32(),
				ZCallBufferSlot.FromFloat(value),
			};
			ZCallBuffer buffer = new(slots, numSlots);
			ZCallHandle handle = alc.GetZCallHandle("up://Script/Engine.CharacterMovementComponent:MaxWalkSpeed");
			alc.ZCall(handle, &buffer);
		}
	}
	
}