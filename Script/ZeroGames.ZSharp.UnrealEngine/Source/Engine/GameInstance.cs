// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public partial class GameInstance
{
	
	public T? GetSubsystem<T>() where T : GameInstanceSubsystem, IStaticClass => SubsystemBlueprintLibrary.GetGameInstanceSubsystem(this, T.StaticClass) as T;
	
	public static GameInstance? PrimaryInstance
	{
		get
		{
			MasterAlcCache.GuardInvariant();
			return InternalPrimaryInstance;
		}
	}

	public static GameInstance PrimaryInstanceChecked => PrimaryInstance ?? throw new InvalidOperationException();

	private static unsafe GameInstance? InternalPrimaryInstance { get; } = GameInstance_Interop.GetPrimaryInstance().GetTarget<GameInstance>();
	
}


