// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public partial class Vector
{

	public Vector(double x, double y, double z) : this()
	{
		X = x;
		Y = y;
		Z = z;
	}

	public override string ToString()
	{
		return $"Vector {{ X={X}, Y={Y}, Z={Z} }}";
	}

	public unsafe double X
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
				ZCallBufferSlot.FromDouble(),
			};
			ZCallBuffer buffer = new(slots, numSlots);
			ZCallHandle handle = alc.GetZCallHandle("up://Script/CoreUObject.Vector:X");
			alc.ZCall(handle, &buffer);

			return slots[3].ReadDouble();
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
				ZCallBufferSlot.FromDouble(value),
			};
			ZCallBuffer buffer = new(slots, numSlots);
			ZCallHandle handle = alc.GetZCallHandle("up://Script/CoreUObject.Vector:X");
			alc.ZCall(handle, &buffer);
		}
	}
	
	public unsafe double Y
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
				ZCallBufferSlot.FromDouble(),
			};
			ZCallBuffer buffer = new(slots, numSlots);
			ZCallHandle handle = alc.GetZCallHandle("up://Script/CoreUObject.Vector:Y");
			alc.ZCall(handle, &buffer);

			return slots[3].ReadDouble();
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
				ZCallBufferSlot.FromDouble(value),
			};
			ZCallBuffer buffer = new(slots, numSlots);
			ZCallHandle handle = alc.GetZCallHandle("up://Script/CoreUObject.Vector:Y");
			alc.ZCall(handle, &buffer);
		}
	}
	
	public unsafe double Z
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
				ZCallBufferSlot.FromDouble(),
			};
			ZCallBuffer buffer = new(slots, numSlots);
			ZCallHandle handle = alc.GetZCallHandle("up://Script/CoreUObject.Vector:Z");
			alc.ZCall(handle, &buffer);

			return slots[3].ReadDouble();
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
				ZCallBufferSlot.FromDouble(value),
			};
			ZCallBuffer buffer = new(slots, numSlots);
			ZCallHandle handle = alc.GetZCallHandle("up://Script/CoreUObject.Vector:Z");
			alc.ZCall(handle, &buffer);
		}
	}
	
}