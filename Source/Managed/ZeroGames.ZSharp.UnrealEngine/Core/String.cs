// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;
using ZeroGames.ZSharp.Core;

namespace ZeroGames.ZSharp.UnrealEngine.Core;

public class String : IConjugate<String>
{

    public static String BuildConjugate(IntPtr unmanaged) => new(unmanaged);

    public String() : this(string.Empty){}

    public String(string content)
    {
        GCHandle = GCHandle.Alloc(this, GCHandleType.Weak);
        _bNative = false;
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
            alc.ZCall("ex://String.Len", &buffer);
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
            alc.ZCall("ex://String.GetData", &buffer);
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
                alc.ZCall("ex://String.SetData", &buffer);
            }
        }
    }

    public GCHandle GCHandle { get; }
    public IntPtr Unmanaged { get; }
    
    private String(IntPtr unmanaged)
    {
        GCHandle = GCHandle.Alloc(this);
        Unmanaged = unmanaged;
        _bNative = true;
    }
    
    ~String()
    {
        if (!_bNative)
        {
            
        }
    }

    private bool _bNative;

}


