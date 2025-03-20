// Copyright Zero Games. All Rights Reserved.

using System.Threading;

namespace ZeroGames.ZSharp.Core.Async;

public interface IGameThreadScheduler
{
	public static IGameThreadScheduler Instance => GameThreadScheduler.Instance;
	void Send(SendOrPostCallback d, object? state);
	void Post(SendOrPostCallback d, object? state);
}


