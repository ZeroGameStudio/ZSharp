// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public partial class UnrealObject
{

    public bool IsA(UnrealClass @class) => this.ZCall("ex://Object.IsA", @class, false)[-1].Bool;
    public bool IsA<T>() where T : UnrealObject => IsA((UnrealClass)typeof(T).GetProperty(nameof(IStaticClass.SStaticClass))!.GetValue(null)!);
    public bool Implements(UnrealClass @interface) => this.ZCall("ex://Object.Implements", @interface, false)[-1].Bool;
    public bool Implements<T>() where T : IUnrealInterface => Implements((UnrealClass)typeof(T).GetProperty(nameof(IStaticClass.SStaticClass))!.GetValue(null)!);

    public void MarkAsGarbage() => this.ZCall("ex://Object.MarkAsGarbage", [ null ]);
    
    public bool __IsGarbage => this.ZCall("ex://Object.IsGarbage", [ null ])[-1].Bool;
    public override UnrealClass __Class => this.ZCall("ex://Object.GetClass", [ null ])[-1].ReadConjugate<UnrealClass>() ?? throw new InvalidOperationException();
    public UnrealObject? __Outer => this.ZCall("ex://Object.GetOuter", [ null ])[-1].ReadConjugate<UnrealObject>();
    public UnrealString __Name => this.ZCall("ex://Object.GetName", [ null ])[-1].ReadConjugate<UnrealString>() ?? throw new InvalidOperationException();
    public UnrealString __PathName => this.ZCall("ex://Object.GetPathName", [ null ])[-1].ReadConjugate<UnrealString>() ?? throw new InvalidOperationException();
    
}


