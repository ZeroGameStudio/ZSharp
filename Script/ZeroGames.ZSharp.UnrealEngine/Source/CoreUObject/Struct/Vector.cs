// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public partial class Vector
{

	public Vector(double x, double y, double z) : this()
	{
		X = x;
		Y = y;
		Z = z;
	}
	
	public override string ToString()
	{
		return $"Vector {{ X={X}, Y={Y}, Z={Z} }}";
	}
	
}


