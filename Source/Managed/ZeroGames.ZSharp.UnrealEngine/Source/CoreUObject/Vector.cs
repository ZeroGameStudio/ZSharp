// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public partial class Vector
{

	public Vector(double x, double y, double z)
	{
		X = x;
		Y = y;
		Z = z;
	}

	public unsafe double X
	{
		get
		{
			IMasterAssemblyLoadContext alc = GetOwningAlc();
			const int32 numSlots = 3;
			ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[numSlots]
			{
				ZCallBufferSlot.FromConjugate(this),
				ZCallBufferSlot.FromBool(false),
				ZCallBufferSlot.FromDouble(0.0),
			};
			ZCallBuffer buffer = new(slots, numSlots);
			ZCallHandle handle = alc.GetZCallHandle("up://Script/CoreUObject.Vector:X");
			alc.ZCall(handle, &buffer);

			return slots[2].ReadDouble();
		}
		set
		{
			IMasterAssemblyLoadContext alc = GetOwningAlc();
			const int32 numSlots = 3;
			ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[numSlots]
			{
				ZCallBufferSlot.FromConjugate(this),
				ZCallBufferSlot.FromBool(true),
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
			const int32 numSlots = 3;
			ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[numSlots]
			{
				ZCallBufferSlot.FromConjugate(this),
				ZCallBufferSlot.FromBool(false),
				ZCallBufferSlot.FromDouble(0.0),
			};
			ZCallBuffer buffer = new(slots, numSlots);
			ZCallHandle handle = alc.GetZCallHandle("up://Script/CoreUObject.Vector:Y");
			alc.ZCall(handle, &buffer);

			return slots[2].ReadDouble();
		}
		set
		{
			IMasterAssemblyLoadContext alc = GetOwningAlc();
			const int32 numSlots = 3;
			ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[numSlots]
			{
				ZCallBufferSlot.FromConjugate(this),
				ZCallBufferSlot.FromBool(true),
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
			const int32 numSlots = 3;
			ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[numSlots]
			{
				ZCallBufferSlot.FromConjugate(this),
				ZCallBufferSlot.FromBool(false),
				ZCallBufferSlot.FromDouble(0.0),
			};
			ZCallBuffer buffer = new(slots, numSlots);
			ZCallHandle handle = alc.GetZCallHandle("up://Script/CoreUObject.Vector:Z");
			alc.ZCall(handle, &buffer);

			return slots[2].ReadDouble();
		}
		set
		{
			IMasterAssemblyLoadContext alc = GetOwningAlc();
			const int32 numSlots = 3;
			ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[numSlots]
			{
				ZCallBufferSlot.FromConjugate(this),
				ZCallBufferSlot.FromBool(true),
				ZCallBufferSlot.FromDouble(value),
			};
			ZCallBuffer buffer = new(slots, numSlots);
			ZCallHandle handle = alc.GetZCallHandle("up://Script/CoreUObject.Vector:Z");
			alc.ZCall(handle, &buffer);
		}
	}
	
}