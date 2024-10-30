// Copyright Zero Games. All Rights Reserved.

using System.Threading;

namespace ZeroGames.ZSharp.Core;

// IMPORTANT: Don't rename because ZeroGames.ZSharp.Core.Async depends on it.
internal static class GameThreadScheduler
{

	public static void Post(SendOrPostCallback d, object? state) => _post(d, state);

	// IMPORTANT: Don't rename because ZeroGames.ZSharp.Core.Async inject this by name.
	private static Action<SendOrPostCallback, object?> _post = null!;

}


