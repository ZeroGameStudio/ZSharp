// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public partial class UEngine
{

	public T? GetSubsystem<T>() where T : UEngineSubsystem, IStaticClass => USubsystemBlueprintLibrary.GetEngineSubsystem(T.StaticClass) as T;

	public static UEngine Instance
	{
		get
		{
			MasterAlcCache.GuardInvariant();
			return InternalInstance;
		}
	}

	private static unsafe UEngine InternalInstance { get; } = Engine_Interop.GetEngine().GetTargetChecked<UEngine>();

}


