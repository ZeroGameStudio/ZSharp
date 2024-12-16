// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public partial class World
{
	
	public T? GetSubsystem<T>() where T : WorldSubsystem, IStaticClass => SubsystemBlueprintLibrary.GetWorldSubsystem(this, T.StaticClass) as T;
	
}


