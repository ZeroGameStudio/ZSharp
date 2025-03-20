// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public partial class LocalPlayer
{
	
	public T? GetSubsystem<T>() where T : LocalPlayerSubsystem, IStaticClass => SubsystemBlueprintLibrary.GetLocalPlayerSubsystem(this, T.StaticClass) as T;

}


