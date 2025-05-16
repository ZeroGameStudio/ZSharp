// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public partial class FQuat
{
	
	public FVector RotateVector(FVector vector) => UKismetMathLibrary.Quat_RotateVector(this, vector);
	public FVector UnrotateVector(FVector vector) => UKismetMathLibrary.Quat_UnrotateVector(this, vector);
	
	public static explicit operator FVector(FQuat @this) => @this.Vector;
	public static explicit operator FRotator(FQuat @this) => @this.Rotator;

	public FVector Vector => Rotator.Vector;
	public FRotator Rotator => UKismetMathLibrary.Quat_Rotator(this);

}


