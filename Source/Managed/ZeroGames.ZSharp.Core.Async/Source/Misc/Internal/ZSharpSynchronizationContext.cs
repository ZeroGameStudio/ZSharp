﻿// Copyright Zero Games. All Rights Reserved.

using System.Collections.Concurrent;
using System.Threading;

namespace ZeroGames.ZSharp.Core.Async;

public sealed class ZSharpSynchronizationContext : SynchronizationContext
{

	public static ZSharpSynchronizationContext Instance { get; } = new();

	public override void Send(SendOrPostCallback d, object? state)
	{
		if (IsInGameThread)
		{
			ProtectedCall(d, state);
		}
		else
		{
			Post(d, state);
		}
	}

	public override void Post(SendOrPostCallback d, object? state) => _recs.Enqueue(new(d, state));

	private void ProtectedCall(SendOrPostCallback d, object? state)
	{
		try
		{
			d(state);
		}
		catch (Exception ex)
		{
			UE_ERROR(LogZSharpScriptAsync, $"Unhandled Exception Detected.\n{ex}");
		}
	}
	
	private readonly record struct Rec(SendOrPostCallback Callback, object? State);

	static ZSharpSynchronizationContext()
	{
		if (AsyncSettings.UseZSharpSynchronizationContext)
		{
			check(Current is null);
			SetSynchronizationContext(Instance);
		}

		IEventLoop.Instance.Register(EEventLoopTickingGroup.RealtimeTick, Tick, null);
	}
	
	private static void Tick(in EventLoopArgs args, object? state)
	{
		while (Instance._recs.TryDequeue(out var rec))
		{
			Instance.ProtectedCall(rec.Callback, rec.State);
		}
	}

	private ConcurrentQueue<Rec> _recs = new();

}


