// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public interface IReactiveLifetimeSource : ILifetimeSource
{
	ReactiveLifetime ReactiveLifetime { get; }
}


