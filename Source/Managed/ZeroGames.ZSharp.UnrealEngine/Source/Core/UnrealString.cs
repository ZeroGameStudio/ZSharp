// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Core;

public partial class UnrealString : PlainUnmanagedClassExportedObjectBase, IConjugate<UnrealString>
{

    public static UnrealString BuildConjugate(IntPtr unmanaged) => new(unmanaged);

    public UnrealString() : this(string.Empty){}

    public unsafe UnrealString(string content)
    {
        IMasterAssemblyLoadContext alc = IMasterAssemblyLoadContext.Get()!;
        Unmanaged = alc.BuildConjugate(2, this);
        if (Unmanaged == IntPtr.Zero)
        {
            throw new InvalidOperationException();
        }

        Data = content;
    }

    public override string ToString()
    {
        return Data;
    }

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

    protected override unsafe void ReleaseUnmanagedResource()
    {
        IMasterAssemblyLoadContext alc = IMasterAssemblyLoadContext.Get()!;
        alc.ReleaseConjugate(2, Unmanaged);
    }

    static UnrealString()
    {
        __sZCallHandle_Dtor = IMasterAssemblyLoadContext.Get()!.GetZCallHandle("ex://String.Dtor");
    }

    private UnrealString(IntPtr unmanaged) : base(unmanaged){}

    private static ZCallHandle __sZCallHandle_Dtor;

}


