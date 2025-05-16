// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public partial class UActorComponent
{

	public T? GetOwner<T>() where T : AActor
	{
		return GetOwner() as T;
	}

	public AActor GetOwnerChecked() => GetOwnerChecked<AActor>();

	public T GetOwnerChecked<T>() where T : AActor
	{
		if (GetOwner() is T owner)
		{
			return owner;
		}

		throw new InvalidOperationException();
	}
	
	public ENetMode GetNetMode()
	{
		MasterAlcCache.GuardInvariant();
		return InternalGetNetMode();
	}

	private unsafe ENetMode InternalGetNetMode()
		=> ActorComponent_Interop.GetNetMode(ConjugateHandle.FromConjugate(this));

}


