// Copyright Zero Games. All Rights Reserved.

using System.Threading;

namespace ZeroGames.ZSharp.Core.Async;

public readonly partial struct ReactiveLifecycle
{

	public static implicit operator ReactiveLifecycle(CancellationToken cancellationToken) => new(cancellationToken);

	public static implicit operator Lifecycle(ReactiveLifecycle @this)
	{
		Thrower.ThrowIfNotInGameThread();
		
		if (@this._backend is null)
		{
			return @this._token.IsInlineExpired ? Lifecycle.Expired : Lifecycle.NeverExpired;
		}

		return Lifecycle.FromBackend(@this._backend);
	}

	public Lifecycle ForceNonReactive() => ((Lifecycle)this).ForceNonReactive();
	
	public LifecycleExpiredRegistration RegisterOnExpired(Action callback)
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

		if (_backend is WeakReference<IReactiveLifecycleBackend> wr)
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
	
	public LifecycleExpiredRegistration RegisterOnExpired(Action<object?> callback, object? state)
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

		if (_backend is WeakReference<IReactiveLifecycleBackend> wr)
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

			if (_backend is WeakReference<IReactiveLifecycleBackend> wr)
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
	
	private ReactiveLifecycle(IReactiveLifecycleBackend backend)
	{
		if (backend.IsExpired(backend.Token))
		{
			_token = LifecycleToken.InlineExpired;
		}
		else
		{
			_backend = new WeakReference<IReactiveLifecycleBackend>(backend);
			_token = backend.Token;
		}
	}

	private ReactiveLifecycle(CancellationTokenSource cts)
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

	private ReactiveLifecycle(CancellationToken cancellationToken)
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

	private ReactiveLifecycle(bool inlineExpired)
	{
		if (inlineExpired)
		{
			_token = LifecycleToken.InlineExpired;
		}
	}

	// WeakReference<IReactiveLifecycleBackend>, CancellationToken or CancellationTokenSource
	private readonly object? _backend;
	private readonly LifecycleToken _token;
	
}


