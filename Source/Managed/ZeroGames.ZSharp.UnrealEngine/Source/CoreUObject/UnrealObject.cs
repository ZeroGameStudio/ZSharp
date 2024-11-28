// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public partial class UnrealObject : IUnrealObject
{

    public bool IsA(UnrealClass @class) => this.ZCall(MasterAlcCache.Instance, "ex://Object.IsA", @class, false)[-1].Bool;
    public bool IsA<T>() where T : UnrealObject => IsA(GetStaticClass<T>());
    public bool Implements(UnrealClass @interface) => this.ZCall(MasterAlcCache.Instance, "ex://Object.Implements", @interface, false)[-1].Bool;
    public bool Implements(Type @interface) => Implements(GetStaticClass(@interface));
    public bool Implements<T>() where T : IUnrealInterface => Implements(typeof(T));

    public void MarkAsGarbage() => this.ZCall(MasterAlcCache.Instance, "ex://Object.MarkAsGarbage", [ null ]);
    
    public bool __IsGarbage => this.ZCall(MasterAlcCache.Instance, "ex://Object.IsGarbage", [ null ])[-1].Bool;
    public override UnrealClass __Class => this.ZCall(MasterAlcCache.Instance, "ex://Object.GetClass", [ null ])[-1].ReadConjugate<UnrealClass>()!;
    public UnrealObject? __Outer => this.ZCall(MasterAlcCache.Instance, "ex://Object.GetOuter", [ null ])[-1].ReadConjugate<UnrealObject>();
    public UnrealString __Name => this.ZCall(MasterAlcCache.Instance, "ex://Object.GetName", [ null ])[-1].ReadConjugate<UnrealString>()!;
    public UnrealString __PathName => this.ZCall(MasterAlcCache.Instance, "ex://Object.GetPathName", [ null ])[-1].ReadConjugate<UnrealString>()!;
    
}


