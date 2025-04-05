// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public partial class ActorComponent
{

	public T? GetOwner<T>() where T : Actor
	{
		return GetOwner() as T;
	}
	
	public ENetMode GetNetMode()
	{
		MasterAlcCache.GuardInvariant();
		return InternalGetNetMode();
	}

	public void SetIsReplicatedByDefault(bool replicated)
	{
		MasterAlcCache.GuardInvariant();
	}
	
	private unsafe ENetMode InternalGetNetMode()
		=> ActorComponent_Interop.GetNetMode(ConjugateHandle.FromConjugate(this));

}


