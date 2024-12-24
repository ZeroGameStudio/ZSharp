// Copyright Zero Games. All Rights Reserved.

using System.Threading;

namespace ZeroGames.ZSharp.Core.Async;

public readonly partial struct ReactiveLifecycle : IEquatable<ReactiveLifecycle>
{

	public bool Equals(ReactiveLifecycle other) => _backend == other._backend && _token == other._token;
	public override bool Equals(object? obj) => obj is ReactiveLifecycle other && Equals(other);
	public override int32 GetHashCode() => _backend?.GetHashCode() ?? 0;
	public static bool operator==(ReactiveLifecycle lhs, ReactiveLifecycle rhs) => lhs.Equals(rhs);
	public static bool operator!=(ReactiveLifecycle lhs, ReactiveLifecycle rhs) => !lhs.Equals(rhs);
	
	public static implicit operator ReactiveLifecycle(CancellationToken cancellationToken) => new(cancellationToken);

	public static implicit operator Lifecycle(ReactiveLifecycle @this)
	{
		Thrower.ThrowIfNotInGameThread();
		
		if (@this.IsExpired)
		{
			return Lifecycle.Expired;
		}

		if (@this._backend is null)
		{
			return Lifecycle.NeverExpired;
		}

		return new(@this._backend);
	}

	public Lifecycle ForceNonReactive() => new(this, true);
	
	public LifecycleExpiredRegistration RegisterOnExpired(Action callback)
	{
		Thrower.ThrowIfNotInGameThread();

		if (IsExpired)
		{
			callback();
			return default;
		}

		if (_backend is IReactiveLifecycleBackend backend)
		{
			return backend.RegisterOnExpired(callback, _token);
		}
		else if (_backend is CancellationTokenSource cts)
		{
			return new(cts.Token.RegisterWithoutCaptureExecutionContext(callback));
		}
		else
		{
			return new(((CancellationToken)_backend!).RegisterWithoutCaptureExecutionContext(callback));
		}
	}
	
	public LifecycleExpiredRegistration RegisterOnExpired(Action<object?> callback, object? state)
	{
		Thrower.ThrowIfNotInGameThread();

		if (IsExpired)
		{
			callback(state);
			return default;
		}

		if (_backend is IReactiveLifecycleBackend backend)
		{
			return backend.RegisterOnExpired(callback, state, _token);
		}
		else if (_backend is CancellationTokenSource cts)
		{
			return new(cts.Token.RegisterWithoutCaptureExecutionContext(callback, state));
		}
		else
		{
			return new(((CancellationToken)_backend!).RegisterWithoutCaptureExecutionContext(callback, state));
		}
	}

	public bool IsExpired
	{
		get
		{
			Thrower.ThrowIfNotInGameThread();

			if (_backend is null)
			{
				return _token.IsInlineExpired;
			}

			if (_token.IsValid)
			{
				var backend = (ILifecycleBackend)_backend;
				return _token != backend.Token || backend.IsExpired(_token);
			}
			else if (_backend is CancellationTokenSource cts)
			{
				return cts.IsCancellationRequested;
			}
			else
			{
				return ((CancellationToken)_backend!).IsCancellationRequested;
			}
		}
	}
	
	internal ReactiveLifecycle(object backend)
	{
		if (backend is IReactiveLifecycleBackend reactiveBackend)
		{
			if (reactiveBackend.IsExpired(reactiveBackend.Token))
			{
				_token = LifecycleToken.InlineExpired;
			}
			else
			{
				_backend = reactiveBackend;
				_token = reactiveBackend.Token;
			}
		}
		else if (backend is CancellationToken cancellationToken)
		{
			if (cancellationToken.IsCancellationRequested)
			{
				_token = LifecycleToken.InlineExpired;
			}
			else if (cancellationToken.CanBeCanceled)
			{
				_backend = cancellationToken;
			}
		}
		else if (backend is CancellationTokenSource cts)
		{
			if (cts.IsCancellationRequested)
			{
				_token = LifecycleToken.InlineExpired;
			}
			else
			{
				_backend = cts;
			}
		}
	}

	internal ReactiveLifecycle(bool inlineExpired)
	{
		if (inlineExpired)
		{
			_token = LifecycleToken.InlineExpired;
		}
	}

	// IReactiveLifecycleBackend, CancellationToken or CancellationTokenSource
	private readonly object? _backend;
	private readonly LifecycleToken _token;
	
}


