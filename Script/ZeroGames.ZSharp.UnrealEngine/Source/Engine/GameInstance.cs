// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public partial class UGameInstance
{
	
	public T? GetSubsystem<T>() where T : UGameInstanceSubsystem, IStaticClass => USubsystemBlueprintLibrary.GetGameInstanceSubsystem(this, T.StaticClass) as T;
	
	public static UGameInstance? PrimaryInstance
	{
		get
		{
			MasterAlcCache.GuardInvariant();
			return InternalPrimaryInstance;
		}
	}

	public static UGameInstance PrimaryInstanceChecked => PrimaryInstance ?? throw new InvalidOperationException();

	private static unsafe UGameInstance? InternalPrimaryInstance { get; } = GameInstance_Interop.GetPrimaryInstance().GetTarget<UGameInstance>();
	
}


