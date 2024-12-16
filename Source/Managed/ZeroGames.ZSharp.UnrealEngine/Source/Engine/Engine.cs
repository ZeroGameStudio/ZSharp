// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public partial class Engine
{

	public T? GetSubsystem<T>() where T : EngineSubsystem, IStaticClass => SubsystemBlueprintLibrary.GetEngineSubsystem(T.StaticClass) as T;

	public static Engine Instance
	{
		get
		{
			MasterAlcCache.GuardInvariant();
			return InternalInstance;
		}
	}

	private static unsafe Engine InternalInstance { get; } = Engine_Interop.GetEngine().GetTargetChecked<Engine>();

}


