// Copyright Zero Games. All Rights Reserved.

using ZeroGames.ZSharp.Core;
using ZeroGames.ZSharp.UnrealEngine.Core;
using ZeroGames.ZSharp.UnrealEngine.Export;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public class UnrealObject : UnrealClassExportedObjectBase, IConjugate<UnrealObject>
{

    public static UnrealObject BuildConjugate(IntPtr unmanaged) => new(unmanaged);

    public unsafe UnrealObject Class
    {
        get
        {
            MasterAssemblyLoadContext alc = MasterAssemblyLoadContext.Get()!;
            ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[]
            {
                new() { Conjugate = ConjugateHandle.FromConjugate(this) },
                new() { Conjugate = default },
            };
            ZCallBuffer buffer = new() { Slots = slots };
            alc.ZCall("ex://Object.GetClass", &buffer);
            return (UnrealObject)slots[1].Conjugate.ToGCHandle().Target!;
        }
    }

    public unsafe UnrealObject? Outer
    {
        get
        {
            MasterAssemblyLoadContext alc = MasterAssemblyLoadContext.Get()!;
            ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[]
            {
                new() { Conjugate = ConjugateHandle.FromConjugate(this) },
                new() { Conjugate = default },
            };
            ZCallBuffer buffer = new() { Slots = slots };
            alc.ZCall("ex://Object.GetOuter", &buffer);
            return slots[1].Conjugate.ToGCHandle().Target as UnrealObject;
        }
    }

    public unsafe UnrealString Name
    {
        get
        {
            MasterAssemblyLoadContext alc = MasterAssemblyLoadContext.Get()!;
            ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[]
            {
                new() { Conjugate = ConjugateHandle.FromConjugate(this) },
                new() { Conjugate = default },
            };
            ZCallBuffer buffer = new() { Slots = slots };
            alc.ZCall("ex://Object.GetName", &buffer);
            return slots[1].Conjugate.ToGCHandle().Target as UnrealString ?? throw new Exception();
        }
    }

    protected override void ReleaseUnmanagedResource()
    {
        Logger.Log($"===================== Release Object {Unmanaged} =====================");
    }

    private UnrealObject(IntPtr unmanaged) : base(unmanaged){}
    
}


