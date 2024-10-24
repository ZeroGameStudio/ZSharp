// Copyright Zero Games. All Rights Reserved.

using System.Collections.Concurrent;
using System.Threading;

namespace ZeroGames.ZSharp.Core.Async;

public sealed class ZSharpSynchronizationContext : SynchronizationContext
{

	public static ZSharpSynchronizationContext Instance { get; }

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

	public void Tick(float deltaTime)
	{
		while (_recs.TryDequeue(out var rec))
		{
			ProtectedCall(rec.Callback, rec.State);
		}
	}

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
		Instance = new();
		check(Current is null);
		SetSynchronizationContext(Instance);
	}

	private ConcurrentQueue<Rec> _recs = new();

}


