// Copyright Zero Games. All Rights Reserved.

using System.Collections.Concurrent;
using System.Threading;

namespace ZeroGames.ZSharp.Core;

internal sealed class ZSharpSynchronizationContext : SynchronizationContext
{

	public override void Send(SendOrPostCallback d, object? state)
	{
		if (Thread.CurrentThread.ManagedThreadId == _threadId)
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
			UE_ERROR($"Unhandled Exception Detected.\n{ex}");
		}
	}
	
	private readonly record struct Rec(SendOrPostCallback Callback, object? State);

	private int32 _threadId = Thread.CurrentThread.ManagedThreadId;
	private ConcurrentQueue<Rec> _recs = new();

}


