// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public partial class Quat
{

	public Rotator ToRotator() => KismetMathLibrary.Quat_Rotator(this);

}


