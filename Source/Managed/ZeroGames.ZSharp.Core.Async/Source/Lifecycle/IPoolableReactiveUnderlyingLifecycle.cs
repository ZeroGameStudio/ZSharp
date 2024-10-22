// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Async;

public interface IPoolableReactiveUnderlyingLifecycle<TImpl> : IPoolableUnderlyingLifecycle<TImpl>, IReactiveUnderlyingLifecycle where TImpl : class, IPoolableUnderlyingLifecycle<TImpl>;


