// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public partial class UnrealObject
{

    public UnrealClass Class => this.ZCall("ex://Object.GetClass", [ null ])[1].ReadConjugate<UnrealClass>() ?? throw new InvalidOperationException();

    public UnrealObject? Outer => this.ZCall("ex://Object.GetOuter", [ null ])[1].ReadConjugate<UnrealObject>();

    public UnrealString Name => this.ZCall("ex://Object.GetName", [ null ])[1].ReadConjugate<UnrealString>() ?? throw new InvalidOperationException();

}


