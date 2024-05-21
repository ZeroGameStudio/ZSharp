// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Core;

public partial class UnrealString
{

    public UnrealString(string content) => Data = content;

    public override string ToString() => Data;

    public unsafe int32 Len
    {
        get
        {
            IMasterAssemblyLoadContext alc = IMasterAssemblyLoadContext.Get()!;
            const int32 numSlots = 2;
            ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[numSlots]
            {
                ZCallBufferSlot.FromConjugate(this),
                ZCallBufferSlot.FromInt32(default),
            };
            ZCallBuffer buffer = new(slots, numSlots);
            ZCallHandle handle = alc.GetZCallHandle("ex://String.Len");
            alc.ZCall(handle, &buffer);
            return slots[1].ReadInt32();
        }
    }
    
    public unsafe string Data
    {
        get
        {
            IMasterAssemblyLoadContext alc = IMasterAssemblyLoadContext.Get()!;
            const int32 numSlots = 2;
            ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[numSlots]
            {
                ZCallBufferSlot.FromConjugate(this),
                ZCallBufferSlot.FromPointer(null),
            };
            ZCallBuffer buffer = new(slots, numSlots);
            ZCallHandle handle = alc.GetZCallHandle("ex://String.GetData");
            alc.ZCall(handle, &buffer);
            return new((char*)slots[1].ReadPointer());
        }
        set
        {
            IMasterAssemblyLoadContext alc = IMasterAssemblyLoadContext.Get()!;
            fixed (char* data = value.ToCharArray())
            {
                const int32 numSlots = 2;
                ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[numSlots]
                {
                    ZCallBufferSlot.FromConjugate(this),
                    ZCallBufferSlot.FromPointer(data),
                };
                ZCallBuffer buffer = new(slots, numSlots);
                ZCallHandle handle = alc.GetZCallHandle("ex://String.SetData");
                alc.ZCall(handle, &buffer);
            }
        }
    }

}


