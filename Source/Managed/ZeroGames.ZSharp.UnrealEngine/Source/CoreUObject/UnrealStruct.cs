// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public partial class UnrealStruct
{

	public bool IsChildOf(UnrealStruct? other) => this.ZCall("ex://Struct.IsChildOf", other, false)[-1].Bool;

}


