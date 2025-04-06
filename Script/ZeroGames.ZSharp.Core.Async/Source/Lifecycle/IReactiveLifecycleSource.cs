// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public interface IReactiveLifecycleSource : ILifecycleSource
{
	ReactiveLifecycle ReactiveLifecycle { get; }
}


