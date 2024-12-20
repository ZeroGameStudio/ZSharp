// Copyright Zero Games. All Rights Reserved.

using System.Collections.Concurrent;
using System.Threading;

namespace ZeroGames.ZSharp.Core.Async;

internal sealed class GameThreadScheduler : IGameThreadScheduler
{

	public static GameThreadScheduler Instance { get; } = new();
	
	public void Send(SendOrPostCallback d, object? state)
	{
		if (IsInGameThread)
		{
			GuardedInvoke(d, state);
		}
		else
		{
			Post(d, state);
		}
	}

	public void Post(SendOrPostCallback d, object? state) => _recs.Enqueue(new(d, state));
	
	private readonly record struct Rec(SendOrPostCallback Callback, object? State);
	
	static GameThreadScheduler()
	{
		IEventLoop.Instance.Register(EEventLoopTickingGroup.RealtimeTick, Tick);
	}
	
	private static void GuardedInvoke(SendOrPostCallback d, object? state)
	{
		try
		{
			d(state);
		}
		catch (Exception ex)
		{
			UnhandledExceptionHelper.Guard(ex, "Unhandled exception detected in Game Thread Scheduler.");
		}
	}
	
	private static void Tick(in EventLoopArgs args, ref bool _)
	{
		while (Instance._recs.TryDequeue(out var rec))
		{
			GuardedInvoke(rec.Callback, rec.State);
		}
	}

	private readonly ConcurrentQueue<Rec> _recs = new();
	
}


