// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public partial class UnrealObject
{

    public override UnrealClass __Class => this.ZCall("ex://Object.GetClass", [ null ])[1].ReadConjugate<UnrealClass>() ?? throw new InvalidOperationException();
    public UnrealObject? __Outer => this.ZCall("ex://Object.GetOuter", [ null ])[1].ReadConjugate<UnrealObject>();
    public UnrealString __Name => this.ZCall("ex://Object.GetName", [ null ])[1].ReadConjugate<UnrealString>() ?? throw new InvalidOperationException();
    public UnrealString __PathName => this.ZCall("ex://Object.GetPathName", [ null ])[1].ReadConjugate<UnrealString>() ?? throw new InvalidOperationException();
    
}


