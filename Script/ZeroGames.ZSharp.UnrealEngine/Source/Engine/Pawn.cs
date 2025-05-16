// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public partial class APawn
{

	public APlayerController? GetPlayerController() => GetController<APlayerController>();
	
	public T? GetController<T>() where T : AController => GetController() as T;

}


