// Copyright Zero Games. All Rights Reserved.

using System.Threading;

namespace ZeroGames.ZSharp.Core.Async;

public readonly partial struct ReactiveLifetime
{

	public static implicit operator ReactiveLifetime(CancellationToken cancellationToken) => new(cancellationToken);

	public static implicit operator Lifetime(ReactiveLifetime @this)
	{
		Thrower.ThrowIfNotInGameThread();
		
		if (@this._backend is null)
		{
			return @this._token.IsInlineExpired ? Lifetime.Expired : Lifetime.NeverExpired;
		}

		return Lifetime.FromBackend(@this._backend);
	}

	public Lifetime ForceNonReactive() => ((Lifetime)this).ForceNonReactive();
	
	public LifetimeExpiredRegistration RegisterOnExpired(Action callback)
	{
		Thrower.ThrowIfNotInGameThread();
		
		if (_backend is null)
		{
			if (_token.IsInlineExpired)
			{
				callback();
			}
			return default;
		}

		if (_backend is WeakReference<IReactiveLifetimeBackend> wr)
		{
			if (!wr.TryGetTarget(out var backend))
			{
				callback();
				return default;
			}
			
			return backend.RegisterOnExpired(callback, _token);
		}
		
		if (_backend is CancellationTokenSource cts)
		{
			return new(cts.Token.RegisterWithoutCaptureExecutionContext(callback));
		}
		
		return new(((CancellationToken)_backend).RegisterWithoutCaptureExecutionContext(callback));
	}
	
	public LifetimeExpiredRegistration RegisterOnExpired(Action<object?> callback, object? state)
	{
		Thrower.ThrowIfNotInGameThread();

		if (_backend is null)
		{
			if (_token.IsInlineExpired)
			{
				callback(state);
			}
			return default;
		}

		if (_backend is WeakReference<IReactiveLifetimeBackend> wr)
		{
			if (!wr.TryGetTarget(out var backend))
			{
				callback(state);
				return default;
			}
			
			return backend.RegisterOnExpired(callback, state, _token);
		}
		
		if (_backend is CancellationTokenSource cts)
		{
			return new(cts.Token.RegisterWithoutCaptureExecutionContext(callback, state));
		}
		
		return new(((CancellationToken)_backend).RegisterWithoutCaptureExecutionContext(callback, state));
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

			if (_backend is WeakReference<IReactiveLifetimeBackend> wr)
			{
				if (!wr.TryGetTarget(out var backend))
				{
					return true;
				}
				
				return backend.IsExpired(_token);
			}
			
			if (_backend is CancellationTokenSource cts)
			{
				return cts.IsCancellationRequested;
			}

			return ((CancellationToken)_backend).IsCancellationRequested;
		}
	}
	
	private ReactiveLifetime(IReactiveLifetimeBackend backend)
	{
		if (backend.IsExpired(backend.Token))
		{
			_token = LifetimeToken.InlineExpired;
		}
		else
		{
			_backend = new WeakReference<IReactiveLifetimeBackend>(backend);
			_token = backend.Token;
		}
	}

	private ReactiveLifetime(CancellationTokenSource cts)
	{
		if (cts.IsCancellationRequested)
		{
			_token = LifetimeToken.InlineExpired;
		}
		else
		{
			_backend = cts;
		}
	}

	private ReactiveLifetime(CancellationToken cancellationToken)
	{
		if (cancellationToken.IsCancellationRequested)
		{
			_token = LifetimeToken.InlineExpired;
		}
		else if (cancellationToken.CanBeCanceled)
		{
			_backend = cancellationToken;
		}
	}

	private ReactiveLifetime(bool inlineExpired)
	{
		if (inlineExpired)
		{
			_token = LifetimeToken.InlineExpired;
		}
	}

	// WeakReference<IReactiveLifetimeBackend>, CancellationToken or CancellationTokenSource
	private readonly object? _backend;
	private readonly LifetimeToken _token;
	
}


