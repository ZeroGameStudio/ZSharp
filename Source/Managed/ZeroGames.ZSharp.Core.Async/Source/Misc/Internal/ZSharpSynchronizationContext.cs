// Copyright Zero Games. All Rights Reserved.

using System.Threading;

namespace ZeroGames.ZSharp.Core.Async;

internal sealed class ZSharpSynchronizationContext : SynchronizationContext
{
	public override void Send(SendOrPostCallback d, object? state) => GameThreadScheduler.Instance.Send(d, state);
	public override void Post(SendOrPostCallback d, object? state) => GameThreadScheduler.Instance.Post(d, state);
}


