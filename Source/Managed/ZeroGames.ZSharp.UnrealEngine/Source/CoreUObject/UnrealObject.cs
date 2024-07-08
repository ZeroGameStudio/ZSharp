// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public partial class UnrealObject
{

    public unsafe UnrealObject Class
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
            ZCallHandle handle = alc.GetZCallHandle("ex://Object.GetClass");
            alc.ZCall(handle, &buffer);
            
            return slots[1].ReadConjugate<UnrealObject>() ?? throw new InvalidOperationException();
        }
    }

    public unsafe UnrealObject? Outer
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
            ZCallHandle handle = alc.GetZCallHandle("ex://Object.GetOuter");
            alc.ZCall(handle, &buffer);
            
            return slots[1].ReadConjugate<UnrealObject>();
        }
    }

    public unsafe UnrealString Name
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
            ZCallHandle handle = alc.GetZCallHandle("ex://Object.GetName");
            alc.ZCall(handle, &buffer);
            
            return slots[1].ReadConjugate<UnrealString>() ?? throw new InvalidOperationException();
        }
    }

}


