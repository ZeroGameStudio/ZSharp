// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public partial class ULocalPlayer
{
	
	public T? GetSubsystem<T>() where T : ULocalPlayerSubsystem, IStaticClass => USubsystemBlueprintLibrary.GetLocalPlayerSubsystem(this, T.StaticClass) as T;

}


