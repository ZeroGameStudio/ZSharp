// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;
using System.Threading;

namespace ZeroGames.ZSharp.Async;

public static class ThreadHelper
{

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void ValidateGameThread(string? message = "Accessing ZSharpAsync component in non-GameThread.")
	{
		if (!IsInGameThread)
		{
			throw new InvalidOperationException(message);
		}
	}

	public static bool IsInGameThread
	{
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		get => UnrealEngineStatics.IsInGameThread;
	}

	public static SynchronizationContext GameThreadSynchronizationContext => IMasterAssemblyLoadContext.Instance!.SynchronizationContext;

}


