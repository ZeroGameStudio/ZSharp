// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public partial class UObject : IUnrealObject, ILifecycleBackend
{

    public UClass GetClass()
    {
        MasterAlcCache.GuardInvariant();
        return InternalGetClass();
    }

    public string GetName()
    {
        MasterAlcCache.GuardInvariant();
        return InternalGetName();
    }

    public string GetPathName()
    {
        MasterAlcCache.GuardInvariant();
        return InternalGetPathName();
    }

    public UObject? GetOuter()
    {
        MasterAlcCache.GuardInvariant();
        return InternalGetOuter();
    }

    public UObject GetOuterChecked()
    {
        verify(GetOuter() is var outer && outer is not null);
        return outer;
    }
    
    public UWorld? GetWorld()
    {
        MasterAlcCache.GuardInvariant();
        return InternalGetWorld();
    }

    public UWorld GetWorldChecked()
    {
        verify(GetWorld() is var world && world is not null);
        return world;
    }

    public bool IsA(UClass @class)
    {
        MasterAlcCache.GuardInvariant();
        return InternalIsA(@class);
    }
    public bool IsA(Type @class) => IsA(UClass.FromType(@class));
    public bool IsA<T>() where T : UObject => IsA(UClass.FromType<T>());

    public bool Implements(UClass @interface) => GetClass().ImplementsInterface(@interface);
    public bool Implements(Type @interface) => Implements(UClass.FromType(@interface));
    public bool Implements<T>() where T : IInterface => Implements(typeof(T));

    public bool IsIn(UObject outer)
    {
        MasterAlcCache.GuardInvariant();
        return InternalIsIn(outer);
    }

    public bool Rename(string? newName, UObject? newOuter)
    {
        MasterAlcCache.GuardInvariant();
        return InternalRename(newName, newOuter);
    }
    public bool Rename(string newName) => Rename(newName, null);
    public bool Rename(UObject newOuter) => Rename(null, newOuter);

    bool ILifecycleBackend.IsExpired(LifecycleToken token) => !__IsValid;

    public bool __IsValid => InternalIsValid;
    public Lifecycle Lifecycle => Lifecycle.FromBackend(this);
    public Lifecycle LifecycleEvenIfGarbage => Lifecycle.FromBackend(_lifecycleBackendEvenIfGarbage ??= new LifecycleBackendEvenIfGarbage(this));
    LifecycleToken ILifecycleBackend.Token { get; } = default(LifecycleToken).Next;

    // @TODO: Construct on async loading thread.
    // Only called on emitted class, just before C++ UObject::PostInitProperties().
    // [ZCall(Name = ".pip")]
    // protected virtual void PostInitProperties(){}

    private class LifecycleBackendEvenIfGarbage(UObject target) : ILifecycleBackend
    {
        bool ILifecycleBackend.IsExpired(LifecycleToken token) => target.IsExpired;
        Lifecycle ILifecycleSource.Lifecycle => Lifecycle.FromBackend(this);
        LifecycleToken ILifecycleBackend.Token { get; } = default(LifecycleToken).Next;
    }

    private unsafe UClass InternalGetClass()
        => UnrealObject_Interop.GetClass(ConjugateHandle.FromConjugate(this)).GetTargetChecked<UClass>();

    private unsafe string InternalGetName()
    {
        using InteropString buffer = new();
        UnrealObject_Interop.GetName(ConjugateHandle.FromConjugate(this), buffer.Address);
        return buffer;
    }

    private unsafe string InternalGetPathName()
    {
        using InteropString buffer = new();
        UnrealObject_Interop.GetPathName(ConjugateHandle.FromConjugate(this), buffer.Address);
        return buffer;
    }
    
    private unsafe UWorld? InternalGetWorld()
        => UnrealObject_Interop.GetWorld(ConjugateHandle.FromConjugate(this)).GetTarget<UWorld>();
    
    private unsafe UObject? InternalGetOuter()
        => UnrealObject_Interop.GetOuter(ConjugateHandle.FromConjugate(this)).GetTarget<UObject>();
    
    private unsafe bool InternalIsA(UClass @class)
        => UnrealObject_Interop.IsA(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(@class)) > 0;
    
    private unsafe bool InternalIsIn(UObject outer)
        => UnrealObject_Interop.IsIn(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(outer)) > 0;

    private unsafe bool InternalRename(string? newName, UObject? newOuter)
    {
        fixed (char* newNameBuffer = newName)
        {
            return UnrealObject_Interop.Rename(ConjugateHandle.FromConjugate(this), newNameBuffer, ConjugateHandle.FromConjugate(newOuter)) > 0;
        }
    }

    private unsafe bool InternalIsValid => UnrealObject_Interop.IsValid(ConjugateHandle.FromConjugate(this)) > 0;

    private LifecycleBackendEvenIfGarbage? _lifecycleBackendEvenIfGarbage;

}


