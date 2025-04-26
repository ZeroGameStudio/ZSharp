// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public struct LifecycleBackendComp(ILifecycleBackend backend)
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
	
	public LifecycleExpiredRegistration RegisterOnExpired(Action<IReactiveLifecycleBackend, object?> callback, object? state, LifecycleToken token)
	{
		ValidateToken(token);
		ValidateReactive();
		_registry ??= new();
		LifecycleExpiredRegistration reg = new((IReactiveLifecycleBackend)_backend, ++_handle);
		_registry[reg] = new(callback, state);

		return reg;
	}

	public void UnregisterOnExpired(LifecycleExpiredRegistration registration, LifecycleToken token)
	{
		ValidateToken(token);
		ValidateReactive();
		_registry?.Remove(registration);
	}

	public bool IsExpired(LifecycleToken token)
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
			var reactiveBackend = (IReactiveLifecycleBackend)_backend;
			foreach (var pair in _registry)
			{
				Rec rec = pair.Value;
				rec.Callback(reactiveBackend, rec.State);
			}
		}
	}

	public LifecycleToken Token { get; private set; }
	
	private void ValidateToken(LifecycleToken token)
	{
		if (token != Token)
		{
			throw new InvalidOperationException("Lifecycle token expired.");
		}
	}

	private void ValidateReactive()
	{
		if (_backend is not IReactiveLifecycleBackend)
		{
			throw new InvalidOperationException("Lifecycle is not reactive.");
		}
	}
	
	private readonly record struct Rec(Action<IReactiveLifecycleBackend, object?> Callback, object? State);

	private readonly ILifecycleBackend _backend = backend;
	private uint64 _handle;
	
	private bool _expired;
	private Dictionary<LifecycleExpiredRegistration, Rec>? _registry;

}


