// Copyright Zero Games. All Rights Reserved.

using ZeroGames.ZSharp.Core;
using ZeroGames.ZSharp.UnrealEngine.Core;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public class UnrealObject : UnrealClassExportedObjectBase, IConjugate<UnrealObject>
{

    public static UnrealObject BuildConjugate(IntPtr unmanaged) => new(unmanaged);

    public unsafe UnrealObject Class
    {
        get
        {
            IMasterAssemblyLoadContext alc = IMasterAssemblyLoadContext.Get()!;
            const int32 numSlots = 2;
            ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[numSlots]
            {
                ZCallBufferSlot.FromConjugate(this),
                ZCallBufferSlot.FromConjugate(null),
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
            IMasterAssemblyLoadContext alc = IMasterAssemblyLoadContext.Get()!;
            const int32 numSlots = 2;
            ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[numSlots]
            {
                ZCallBufferSlot.FromConjugate(this),
                ZCallBufferSlot.FromConjugate(null),
            };
            ZCallBuffer buffer = new(slots, numSlots);
            ZCallHandle handle = alc.GetZCallHandle("ex://Object.GetOuter");
            alc.ZCall(handle, &buffer);
            return slots[1].ReadConjugate<UnrealObject>() ?? throw new InvalidOperationException();
        }
    }

    public unsafe UnrealString Name
    {
        get
        {
            IMasterAssemblyLoadContext alc = IMasterAssemblyLoadContext.Get()!;
            const int32 numSlots = 2;
            ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[numSlots]
            {
                ZCallBufferSlot.FromConjugate(this),
                ZCallBufferSlot.FromConjugate(null),
            };
            ZCallBuffer buffer = new(slots, numSlots);
            ZCallHandle handle = alc.GetZCallHandle("ex://Object.GetName");
            alc.ZCall(handle, &buffer);
            return slots[1].ReadConjugate<UnrealString>() ?? throw new InvalidOperationException();
        }
    }

    private UnrealObject(IntPtr unmanaged) : base(unmanaged){}
    
}


