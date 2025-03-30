// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public partial class UnrealObject : IUnrealObject
{

    public UnrealClass GetClass()
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

    public UnrealObject? GetOuter()
    {
        MasterAlcCache.GuardInvariant();
        return InternalGetOuter();
    }

    public UnrealObject GetOuterChecked()
    {
        verify(GetOuter() is var outer && outer is not null);
        return outer;
    }
    
    public World? GetWorld()
    {
        MasterAlcCache.GuardInvariant();
        return InternalGetWorld();
    }

    public bool IsA(UnrealClass @class)
    {
        MasterAlcCache.GuardInvariant();
        return InternalIsA(@class);
    }
    public bool IsA(Type @class) => IsA(UnrealClass.FromType(@class));
    public bool IsA<T>() where T : UnrealObject => IsA(UnrealClass.FromType<T>());

    public bool Implements(UnrealClass @interface) => GetClass().ImplementsInterface(@interface);
    public bool Implements(Type @interface) => Implements(UnrealClass.FromType(@interface));
    public bool Implements<T>() where T : IUnrealInterface => Implements(typeof(T));

    public bool IsIn(UnrealObject outer)
    {
        MasterAlcCache.GuardInvariant();
        return InternalIsIn(outer);
    }

    public bool Rename(string? newName, UnrealObject? newOuter)
    {
        MasterAlcCache.GuardInvariant();
        return InternalRename(newName, newOuter);
    }
    public bool Rename(string newName) => Rename(newName, null);
    public bool Rename(UnrealObject newOuter) => Rename(null, newOuter);

    private unsafe UnrealClass InternalGetClass()
        => UnrealObject_Interop.GetClass(ConjugateHandle.FromConjugate(this)).GetTargetChecked<UnrealClass>();

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
    
    private unsafe World? InternalGetWorld()
        => UnrealObject_Interop.GetWorld(ConjugateHandle.FromConjugate(this)).GetTarget<World>();
    
    private unsafe UnrealObject? InternalGetOuter()
        => UnrealObject_Interop.GetOuter(ConjugateHandle.FromConjugate(this)).GetTarget<UnrealObject>();
    
    private unsafe bool InternalIsA(UnrealClass @class)
        => UnrealObject_Interop.IsA(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(@class)) > 0;
    
    private unsafe bool InternalIsIn(UnrealObject outer)
        => UnrealObject_Interop.IsIn(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(outer)) > 0;

    private unsafe bool InternalRename(string? newName, UnrealObject? newOuter)
    {
        fixed (char* newNameBuffer = newName)
        {
            return UnrealObject_Interop.Rename(ConjugateHandle.FromConjugate(this), newNameBuffer, ConjugateHandle.FromConjugate(newOuter)) > 0;
        }
    }

}


