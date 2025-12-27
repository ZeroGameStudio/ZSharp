// Copyright Zero Games. All Rights Reserved.

using ZeroGames.Extensions.Math;

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public partial class AActor
{
	
	public ENetMode GetNetMode()
	{
		MasterAlcCache.GuardInvariant();
		return InternalGetNetMode();
	}
	
	internal void FinishSpawning(in Transform transform)
	{
		MasterAlcCache.GuardInvariant();
		InternalFinishSpawning(transform);
	}
	
	private unsafe void InternalFinishSpawning(in Transform transform)
		=> Actor_Interop.FinishSpawning(ConjugateHandle.FromConjugate(this), transform);
	
	private unsafe ENetMode InternalGetNetMode()
		=> Actor_Interop.GetNetMode(ConjugateHandle.FromConjugate(this));
	
}


