// Copyright Zero Games. All Rights Reserved.

using System.Collections.Concurrent;
using System.Runtime.CompilerServices;
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

	public void Post(SendOrPostCallback d, object? state)
	{
		bool lockTaken = false;
		try
		{
			_recsLock.Enter(ref lockTaken);
			
			_backgroundRecs.Enqueue(new(d, state));
		}
		finally
		{
			if (lockTaken)
			{
				_recsLock.Exit();
			}
		}
	}
	
	private readonly record struct Rec(SendOrPostCallback Callback, object? State);
	
	static GameThreadScheduler()
	{
		IMasterAssemblyLoadContext.RegisterUnloading(Reinitialize, 1);
		Reinitialize();
	}

	private GameThreadScheduler()
	{
		_foregroundRecs = _recs1;
		_backgroundRecs = _recs2;
	}

	private static void Reinitialize()
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
		Instance.SwapRecs();
		while (Instance._foregroundRecs.TryDequeue(out var rec))
		{
			GuardedInvoke(rec.Callback, rec.State);
		}
	}

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	private void SwapRecs()
	{
		bool lockTaken = false;
		try
		{
			_recsLock.Enter(ref lockTaken);

			(_foregroundRecs, _backgroundRecs) = (_backgroundRecs, _foregroundRecs);
		}
		finally
		{
			if (lockTaken)
			{
				_recsLock.Exit();
			}
		}
	}

	private readonly ConcurrentQueue<Rec> _recs1 = new();
	private readonly ConcurrentQueue<Rec> _recs2 = new();

	private ConcurrentQueue<Rec> _foregroundRecs;
	private ConcurrentQueue<Rec> _backgroundRecs;

	private SpinLock _recsLock;

}


