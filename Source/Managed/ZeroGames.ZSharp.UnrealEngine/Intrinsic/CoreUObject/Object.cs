// Copyright Zero Games. All Rights Reserved.

using ZeroGames.ZSharp.Core;
using ZeroGames.ZSharp.UnrealEngine.Export;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public class Object : UnrealClassExportedObjectBase, IConjugate<Object>
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
                new(),
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
                new(),
            };
            ZCallBuffer buffer = new() { Slots = slots };
            alc.ZCall("ex://Object.GetOuter", &buffer);
            return (Object)slots[1].Conjugate.ToGCHandle().Target!;
        }
    }

    public unsafe UnrealString Name
    {
        get
        {
            MasterAssemblyLoadContext alc = MasterAssemblyLoadContext.Get()!;
            UnrealString str = new();
            ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[]
            {
                new() { Conjugate = ConjugateHandle.FromConjugate(this) },
                new() { Conjugate = ConjugateHandle.FromConjugate(str) },
            };
            ZCallBuffer buffer = new() { Slots = slots };
            alc.ZCall("ex://Object.GetName", &buffer);
            return str;
        }
    }

    protected override void ReleaseUnmanagedResource()
    {
        Logger.Log($"===================== Release Object {Unmanaged} =====================");
    }

    private Object(IntPtr unmanaged) : base(unmanaged){}
    
}


