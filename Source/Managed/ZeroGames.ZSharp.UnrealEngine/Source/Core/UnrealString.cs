// Copyright Zero Games. All Rights Reserved.

using ZeroGames.ZSharp.Core;

namespace ZeroGames.ZSharp.UnrealEngine.Core;

public class UnrealString : PlainUnmanagedClassExportedObjectBase, IConjugate<UnrealString>
{

    public static UnrealString BuildConjugate(IntPtr unmanaged) => new(unmanaged);

    public UnrealString() : this(string.Empty){}

    public unsafe UnrealString(string content)
    {
        MasterAssemblyLoadContext alc = MasterAssemblyLoadContext.Get()!;
        fixed (char* data = content.ToCharArray())
        {
            const int32 numSlots = 3;
            ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[numSlots]
            {
                ZCallBufferSlot.FromConjugate(this),
                ZCallBufferSlot.FromPointer(data),
                ZCallBufferSlot.FromPointer(null),
            };
            ZCallBuffer buffer = new(slots, numSlots);
            alc.ZCall("ex://String.Ctor", &buffer, out var handle);
            Unmanaged = slots[2].ReadPointer();
        }
    }

    public override string ToString()
    {
        return Data;
    }

    public unsafe int32 Len
    {
        get
        {
            MasterAssemblyLoadContext alc = MasterAssemblyLoadContext.Get()!;
            const int32 numSlots = 2;
            ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[numSlots]
            {
                ZCallBufferSlot.FromConjugate(this),
                ZCallBufferSlot.FromInt32(default),
            };
            ZCallBuffer buffer = new(slots, numSlots);
            alc.ZCall("ex://String.Len", &buffer, out var handle);
            return slots[1].ReadInt32();
        }
    }
    
    public unsafe string Data
    {
        get
        {
            MasterAssemblyLoadContext alc = MasterAssemblyLoadContext.Get()!;
            const int32 numSlots = 2;
            ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[numSlots]
            {
                ZCallBufferSlot.FromConjugate(this),
                ZCallBufferSlot.FromPointer(null),
            };
            ZCallBuffer buffer = new(slots, numSlots);
            alc.ZCall("ex://String.GetData", &buffer, out var handle);
            return new((char*)slots[1].ReadPointer());
        }
        set
        {
            MasterAssemblyLoadContext alc = MasterAssemblyLoadContext.Get()!;
            fixed (char* data = value.ToCharArray())
            {
                const int32 numSlots = 2;
                ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[numSlots]
                {
                    ZCallBufferSlot.FromConjugate(this),
                    ZCallBufferSlot.FromPointer(data),
                };
                ZCallBuffer buffer = new(slots, numSlots);
                alc.ZCall("ex://String.SetData", &buffer, out var handle);
            }
        }
    }

    protected override unsafe void ReleaseUnmanagedResource()
    {
        MasterAssemblyLoadContext alc = MasterAssemblyLoadContext.Get()!;
        const int32 numSlots = 1;
        ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[numSlots]
        {
            ZCallBufferSlot.FromConjugate(this),
        };
        ZCallBuffer buffer = new(slots, numSlots);
        alc.ZCall_AnyThread(__sZCallHandle_Dtor, &buffer, 1);
    }

    static UnrealString()
    {
        __sZCallHandle_Dtor = MasterAssemblyLoadContext.Get()!.PrecacheZCall("ex://String.Dtor");
    }

    private UnrealString(IntPtr unmanaged) : base(unmanaged){}

    private static ZCallHandle __sZCallHandle_Dtor;

}


