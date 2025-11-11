// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public struct LifetimeBackendComp(ILifetimeBackend backend)
{

	public void Initialize()
	{
		_expired = false;
		_handle = 0;
		_registry?.Clear();
	}

	public void Deinitialize()
	{
		Token = Token.Next;
	}
	
	public LifetimeExpiredRegistration RegisterOnExpired(Action<IReactiveLifetimeBackend, object?> callback, object? state, LifetimeToken token)
	{
		ValidateToken(token);
		ValidateReactive();
		_registry ??= new();
		LifetimeExpiredRegistration reg = new((IReactiveLifetimeBackend)_backend, ++_handle);
		_registry[reg] = new(callback, state);

		return reg;
	}

	public void UnregisterOnExpired(LifetimeExpiredRegistration registration, LifetimeToken token)
	{
		ValidateToken(token);
		ValidateReactive();
		_registry?.Remove(registration);
	}

	public bool IsExpired(LifetimeToken token)
	{
		ValidateToken(token);
		return _expired;
	}

	public void SetExpired()
	{
		check(!_expired);
		
		_expired = true;
		if (_registry is not null)
		{
			var reactiveBackend = (IReactiveLifetimeBackend)_backend;
			foreach (var pair in _registry)
			{
				Rec rec = pair.Value;
				rec.Callback(reactiveBackend, rec.State);
			}
		}
	}

	public LifetimeToken Token { get; private set; }
	
	private void ValidateToken(LifetimeToken token)
	{
		if (token != Token)
		{
			throw new InvalidOperationException("Lifetime token expired.");
		}
	}

	private void ValidateReactive()
	{
		if (_backend is not IReactiveLifetimeBackend)
		{
			throw new InvalidOperationException("Lifetime is not reactive.");
		}
	}
	
	private readonly record struct Rec(Action<IReactiveLifetimeBackend, object?> Callback, object? State);

	private readonly ILifetimeBackend _backend = backend;
	private uint64 _handle;
	
	private bool _expired;
	private Dictionary<LifetimeExpiredRegistration, Rec>? _registry;

}


