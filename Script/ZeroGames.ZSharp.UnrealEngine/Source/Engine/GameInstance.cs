// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public partial class GameInstance
{
	
	public T? GetSubsystem<T>() where T : GameInstanceSubsystem, IStaticClass => SubsystemBlueprintLibrary.GetGameInstanceSubsystem(this, T.StaticClass) as T;
	
}


