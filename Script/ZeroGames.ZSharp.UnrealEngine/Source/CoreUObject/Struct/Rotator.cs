// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public partial class Rotator
{
	
	public Rotator(double pitch, double yaw, double roll) : this()
	{
		Pitch = pitch;
		Yaw = yaw;
		Roll = roll;
	}
	
	public override string ToString()
	{
		return $"Rotator {{ Pitch={Pitch}, Yaw={Yaw}, Roll={Roll} }}";
	}

	public Vector RotateVector(Vector vector) => KismetMathLibrary.Quat_RotateVector(Quat, vector);
	public Vector UnrotateVector(Vector vector) => KismetMathLibrary.Quat_UnrotateVector(Quat, vector);
	
	public static explicit operator Vector(Rotator @this) => @this.Vector;
	public static explicit operator Quat(Rotator @this) => @this.Quat;

	public Vector Vector => KismetMathLibrary.Conv_RotatorToVector(this);
	public Quat Quat => KismetMathLibrary.Conv_RotatorToQuaternion(this);

}

