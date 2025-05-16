// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public partial class APlayerController
{

	public ULocalPlayer? LocalPlayer => Player as ULocalPlayer;
	
	public T? GetSubsystem<T>() where T : ULocalPlayerSubsystem, IStaticClass => USubsystemBlueprintLibrary.GetLocalPlayerSubSystemFromPlayerController(this, T.StaticClass) as T;
	
}


