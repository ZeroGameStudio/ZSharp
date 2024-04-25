// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;
using ZeroGames.ZSharp.Core;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public class Object : IConjugate<Object>
{

    public static Object BuildConjugate(IntPtr unmanaged) => new(unmanaged);

    public unsafe Object Class
    {
        get
        {
            MasterAssemblyLoadContext alc = MasterAssemblyLoadContext.Get()!;
            ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[]
            {
                new() { Conjugate = ConjugateHandle.FromConjugate(this) },
                new() { Conjugate = new() },
            };
            ZCallBuffer buffer = new() { Slots = slots };
            alc.ZCall("ex://Object.GetClass", &buffer);
            return (Object)slots[1].Conjugate.ToGCHandle().Target!;
        }
    }

    public unsafe Object? Outer
    {
        get
        {
            MasterAssemblyLoadContext alc = MasterAssemblyLoadContext.Get()!;
            ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[]
            {
                new() { Conjugate = ConjugateHandle.FromConjugate(this) },
                new() { Conjugate = new() },
            };
            ZCallBuffer buffer = new() { Slots = slots };
            alc.ZCall("ex://Object.GetOuter", &buffer);
            return (Object)slots[1].Conjugate.ToGCHandle().Target!;
        }
    }

    public unsafe string Name
    {
        get
        {
            MasterAssemblyLoadContext alc = MasterAssemblyLoadContext.Get()!;
            ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[]
            {
                new() { Conjugate = ConjugateHandle.FromConjugate(this) },
                new() { Conjugate = new() },
            };
            ZCallBuffer buffer = new() { Slots = slots };
            alc.ZCall("ex://Object.GetName", &buffer);
            return ((UnrealString)slots[1].Conjugate.ToGCHandle().Target!).Data;
        }
    }
    
    public GCHandle GCHandle { get; }
    public IntPtr Unmanaged { get; }

    private Object(IntPtr unmanaged)
    {
        GCHandle = GCHandle.Alloc(this);
        Unmanaged = unmanaged;
    }
    
}


