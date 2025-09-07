// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public partial class UEngine
{
	
	public T? GetSubsystem<T>() where T : UEngineSubsystem, IStaticClass => USubsystemBlueprintLibrary.GetEngineSubsystem(T.StaticClass) as T;
	
	public static bool IsInitialized
	{
		get
		{
			MasterAlcCache.GuardInvariant();
			return InternalIsInitialized;
		}
	}
	
	public static UEngine Instance
	{
		get
		{
			MasterAlcCache.GuardInvariant();
			return InternalInstance;
		}
	}

	private static unsafe bool InternalIsInitialized => Engine_Interop.IsInitialized() > 0;
	[field: AllowNull]
	private static unsafe UEngine InternalInstance => field ??= Engine_Interop.GetEngine().GetTargetChecked<UEngine>(); // Lazy because static ctor can run before engine inits.

}


