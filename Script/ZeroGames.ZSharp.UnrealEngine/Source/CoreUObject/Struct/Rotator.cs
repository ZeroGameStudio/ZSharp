// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public partial class FRotator
{
	
	public FRotator(double pitch, double yaw, double roll) : this()
	{
		Pitch = pitch;
		Yaw = yaw;
		Roll = roll;
	}
	
	public void Deconstruct(out double pitch, out double yaw, out double roll)
	{
		pitch = Pitch;
		yaw = Yaw;
		roll = Roll;
	}
	public void Deconstruct(out double pitch, out double yaw) => Deconstruct(out pitch, out yaw, out _);
	
	public override string ToString()
	{
		return $"Rotator {{ Pitch={Pitch}, Yaw={Yaw}, Roll={Roll} }}";
	}

	public FVector RotateVector(FVector vector) => UKismetMathLibrary.Quat_RotateVector(Quat, vector);
	public FVector UnrotateVector(FVector vector) => UKismetMathLibrary.Quat_UnrotateVector(Quat, vector);
	
	public static explicit operator FVector(FRotator @this) => @this.Vector;
	public static explicit operator FQuat(FRotator @this) => @this.Quat;

	public FVector Vector => UKismetMathLibrary.Conv_RotatorToVector(this);
	public FQuat Quat => UKismetMathLibrary.Conv_RotatorToQuaternion(this);

}

