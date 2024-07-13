// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public partial class Character
{

	public bool SetActorLocation(Vector location, bool sweep, out HitResult sweepHitResult, bool isTeleport)
	{
		sweepHitResult = new();

		return CallUnrealFunction<bool>("K2_SetActorLocation", location, sweep, sweepHitResult, isTeleport, false);
	}

	public void Jump() => CallUnrealFunction("Jump");
	
}