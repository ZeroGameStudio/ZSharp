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

	public bool IsActorInitialized()
	{
		MasterAlcCache.GuardInvariant();
		return InternalIsActorInitialized();
	}
	
	public bool IsActorBeginningPlay()
	{
		MasterAlcCache.GuardInvariant();
		return InternalIsActorBeginningPlay();
	}
	
	public bool HasActorBegunPlay()
	{
		MasterAlcCache.GuardInvariant();
		return InternalHasActorBegunPlay();
	}
	
	internal void FinishSpawning(in Transform transform, bool requiresRootComponent)
	{
		MasterAlcCache.GuardInvariant();
		InternalFinishSpawning(transform, requiresRootComponent);
	}
	
	private unsafe void InternalFinishSpawning(in Transform transform, bool requiresRootComponent)
		=> Actor_Interop.FinishSpawning(ConjugateHandle.FromConjugate(this), transform, Convert.ToByte(requiresRootComponent));
	
	private unsafe ENetMode InternalGetNetMode()
		=> Actor_Interop.GetNetMode(ConjugateHandle.FromConjugate(this));

	private unsafe bool InternalIsActorInitialized()
		=> Actor_Interop.IsActorInitialized(ConjugateHandle.FromConjugate(this)) > 0;
	
	private unsafe bool InternalIsActorBeginningPlay()
		=> Actor_Interop.IsActorBeginningPlay(ConjugateHandle.FromConjugate(this)) > 0;
	
	private unsafe bool InternalHasActorBegunPlay()
		=> Actor_Interop.HasActorBegunPlay(ConjugateHandle.FromConjugate(this)) > 0;
	
}


