// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;
using ZeroGames.ZSharp.Core;
using ZeroGames.ZSharp.UnrealEngine.Export;

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
            ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[]
            {
                new() { Conjugate = ConjugateHandle.FromConjugate(this) },
                new() { Pointer = (IntPtr)data },
                new(),
            };
            ZCallBuffer buffer = new() { Slots = slots };
            alc.ZCall("ex://String.Ctor", &buffer, out var handle);
            Unmanaged = buffer.Slots[2].Pointer;
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
            ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[]
            {
                new() { Conjugate = ConjugateHandle.FromConjugate(this) },
                new() { Int32 = 0 },
            };
            ZCallBuffer buffer = new() { Slots = slots };
            alc.ZCall("ex://String.Len", &buffer, out var handle);
            return slots[1].Int32;
        }
    }
    
    public unsafe string Data
    {
        get
        {
            MasterAssemblyLoadContext alc = MasterAssemblyLoadContext.Get()!;
            ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[]
            {
                new() { Conjugate = ConjugateHandle.FromConjugate(this) },
                new() { Pointer = IntPtr.Zero },
            };
            ZCallBuffer buffer = new() { Slots = slots };
            alc.ZCall("ex://String.GetData", &buffer, out var handle);
            return new((char*)slots[1].Pointer);
        }
        set
        {
            MasterAssemblyLoadContext alc = MasterAssemblyLoadContext.Get()!;
            fixed (char* data = value.ToCharArray())
            {
                ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[]
                {
                    new() { Conjugate = ConjugateHandle.FromConjugate(this) },
                    new() { Pointer = (IntPtr)data },
                };
                ZCallBuffer buffer = new() { Slots = slots };
                alc.ZCall("ex://String.SetData", &buffer, out var handle);
            }
        }
    }

    protected override unsafe void ReleaseUnmanagedResource()
    {
        MasterAssemblyLoadContext alc = MasterAssemblyLoadContext.Get()!;
        ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[]
        {
            new() { Conjugate = ConjugateHandle.FromConjugate(this) },
        };
        ZCallBuffer buffer = new() { Slots = slots };
        alc.ZCall_AnyThread(__sZCallHandle_Dtor, &buffer, 1);
    }

    static UnrealString()
    {
        __sZCallHandle_Dtor = MasterAssemblyLoadContext.Get()!.PrecacheZCall("ex://String.Dtor");
    }

    private UnrealString(IntPtr unmanaged) : base(unmanaged){}

    private static ZCallHandle __sZCallHandle_Dtor;

}


