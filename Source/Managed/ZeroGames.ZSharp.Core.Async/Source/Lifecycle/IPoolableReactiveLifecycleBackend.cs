// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public interface IPoolableReactiveLifecycleBackend<TImpl> : IPoolableLifecycleBackend<TImpl>, IReactiveLifecycleBackend where TImpl : class, IPoolableLifecycleBackend<TImpl>;


