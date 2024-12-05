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
	
}

