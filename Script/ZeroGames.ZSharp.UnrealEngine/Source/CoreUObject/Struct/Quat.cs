// Copyright Zero Games. All Rights Reserved.

using ZeroGames.Extensions.Math;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public partial class FQuat
{

	public FQuat(double x, double y, double z, double w) : this()
	{
		X = x;
		Y = y;
		Z = z;
		W = w;
	}
	
	public static implicit operator FQuat(Quaternion value) => new(value.X, value.Y, value.Z, value.W);
	public static implicit operator Quaternion(FQuat value) => new(value.X, value.Y, value.Z, value.W);
	
	public FVector RotateVector(FVector vector) => UKismetMathLibrary.Quat_RotateVector(this, vector);
	public FVector UnrotateVector(FVector vector) => UKismetMathLibrary.Quat_UnrotateVector(this, vector);
	
	public FVector Vector => Rotator.Vector;
	public FRotator Rotator => UKismetMathLibrary.Quat_Rotator(this);

}


