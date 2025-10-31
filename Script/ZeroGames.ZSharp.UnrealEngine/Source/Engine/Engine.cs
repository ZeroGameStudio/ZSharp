// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public partial class UEngine
{
	
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

	public static ZeroTask WhenEngineInitialized { get; }
	
	public static event Action? PostEngineInit
	{
		add
		{
			if (IsInitialized)
			{
				return;
			}

			_postEngineInit += value;
		}
		remove => _postEngineInit -= value;
	}
	
	public T? GetSubsystem<T>() where T : UEngineSubsystem, IStaticClass => USubsystemBlueprintLibrary.GetEngineSubsystem(T.StaticClass) as T;

	internal static void InternalPostEngineInit()
	{
		_engineInitializedTcs?.SetResult();
		_postEngineInit?.Invoke();
	}

	static UEngine()
	{
		if (!IsInitialized)
		{
			_engineInitializedTcs = ZeroTaskCompletionSource.Create();
			WhenEngineInitialized = _engineInitializedTcs.Value.Task.Memoize();
		}
		else
		{
			WhenEngineInitialized = ZeroTask.CompletedTask;
		}
	}

	private static unsafe bool InternalIsInitialized => Engine_Interop.IsInitialized() > 0;
	[field: AllowNull]
	private static unsafe UEngine InternalInstance => field ??= Engine_Interop.GetEngine().GetTargetChecked<UEngine>(); // Lazy because static ctor can run before engine inits.

	private static readonly ZeroTaskCompletionSource? _engineInitializedTcs;
	private static Action? _postEngineInit;

}


