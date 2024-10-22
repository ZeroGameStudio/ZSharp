// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;

namespace ZeroGames.ZSharp.Core.Async;

public struct UnderlyingLifecycleComponent(IUnderlyingLifecycle lifecycle)
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
	
	public LifecycleExpiredRegistration RegisterOnExpired(Action<IReactiveUnderlyingLifecycle, object?> callback, object? state, UnderlyingLifecycleToken token)
	{
		ValidateToken(token);
		ValidateReactive();
		_registry ??= new();
		LifecycleExpiredRegistration reg = new(new(Unsafe.As<IReactiveUnderlyingLifecycle>(_lifecycle)), ++_handle);
		_registry[reg] = new(callback, state);

		return reg;
	}

	public void UnregisterOnExpired(LifecycleExpiredRegistration registration, UnderlyingLifecycleToken token)
	{
		ValidateToken(token);
		ValidateReactive();
		_registry?.Remove(registration);
	}

	public bool IsExpired(UnderlyingLifecycleToken token)
	{
		ValidateToken(token);
		return _expired;
	}

	public void SetExpired()
	{
		if (_expired)
		{
			throw new InvalidOperationException();
		}
		
		_expired = true;
		if (_registry is not null)
		{
			var reactiveUnderlyingLifecycle = Unsafe.As<IReactiveUnderlyingLifecycle>(_lifecycle);
			foreach (var pair in _registry)
			{
				Rec rec = pair.Value;
				rec.Callback(reactiveUnderlyingLifecycle, rec.State);
			}
		}
	}

	public UnderlyingLifecycleToken Token { get; private set; }
	
	private void ValidateToken(UnderlyingLifecycleToken token)
	{
		if (token != Token)
		{
			throw new InvalidOperationException();
		}
	}

	private void ValidateReactive()
	{
		if (_lifecycle is not IReactiveUnderlyingLifecycle)
		{
			throw new InvalidOperationException();
		}
	}
	
	private readonly record struct Rec(Action<IReactiveUnderlyingLifecycle, object?> Callback, object? State);

	private IUnderlyingLifecycle _lifecycle = lifecycle;
	private uint64 _handle;
	
	private bool _expired;
	private Dictionary<LifecycleExpiredRegistration, Rec>? _registry;

}


