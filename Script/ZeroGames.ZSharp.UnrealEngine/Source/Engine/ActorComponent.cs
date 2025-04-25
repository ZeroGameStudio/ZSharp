// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public partial class ActorComponent
{

	public T? GetOwner<T>() where T : Actor
	{
		return GetOwner() as T;
	}

	public Actor GetOwnerChecked() => GetOwnerChecked<Actor>();

	public T GetOwnerChecked<T>() where T : Actor
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


