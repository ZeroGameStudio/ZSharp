// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public partial class ActorComponent
{

	public T? GetOwner<T>() where T : Actor
	{
		return GetOwner() as T;
	}
	
}


