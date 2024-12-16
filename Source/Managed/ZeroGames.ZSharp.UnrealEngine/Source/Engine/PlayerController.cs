// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public partial class PlayerController
{
	
	public T? GetSubsystem<T>() where T : LocalPlayerSubsystem, IStaticClass => SubsystemBlueprintLibrary.GetLocalPlayerSubSystemFromPlayerController(this, T.StaticClass) as T;
	
}


