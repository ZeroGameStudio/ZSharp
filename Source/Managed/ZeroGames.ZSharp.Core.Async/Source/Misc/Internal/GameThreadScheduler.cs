﻿// Copyright Zero Games. All Rights Reserved.

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
			d(state);
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
		IEventLoop.Instance.Register(EEventLoopTickingGroup.RealtimeTick, Tick, null);
	}
	
	private static void Tick(in EventLoopArgs args, object? state)
	{
		while (Instance._recs.TryDequeue(out var rec))
		{
			rec.Callback(rec.State);
		}
	}

	private readonly ConcurrentQueue<Rec> _recs = new();
	
}

