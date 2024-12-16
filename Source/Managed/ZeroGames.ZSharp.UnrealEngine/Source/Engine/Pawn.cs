// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public partial class Pawn
{

	public PlayerController? GetPlayerController() => GetController<PlayerController>();
	
	public T? GetController<T>() where T : Controller => GetController() as T;

}


