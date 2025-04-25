// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public partial class Quat
{
	
	public Vector RotateVector(Vector vector) => KismetMathLibrary.Quat_RotateVector(this, vector);
	public Vector UnrotateVector(Vector vector) => KismetMathLibrary.Quat_UnrotateVector(this, vector);
	
	public static explicit operator Vector(Quat @this) => @this.Vector;
	public static explicit operator Rotator(Quat @this) => @this.Rotator;

	public Vector Vector => Rotator.Vector;
	public Rotator Rotator => KismetMathLibrary.Quat_Rotator(this);

}


