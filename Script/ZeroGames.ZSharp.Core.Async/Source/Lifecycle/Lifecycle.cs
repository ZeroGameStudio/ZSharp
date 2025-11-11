// Copyright Zero Games. All Rights Reserved.

using System.Threading;

namespace ZeroGames.ZSharp.Core.Async;

public readonly partial struct Lifecycle
{

	public static implicit operator Lifecycle(CancellationTokenSource cts) => new(cts);
	public static implicit operator Lifecycle(CancellationToken cancellationToken) => new(cancellationToken);

	public Lifecycle ForceNonReactive() => new(this, true);

	public bool TryToReactive(out ReactiveLifecycle reactiveLifecycle)
	{
		Thrower.ThrowIfNotInGameThread();

		// Never or Expired.
		if (_backend is null)
		{
			reactiveLifecycle = _token.IsInlineExpired ? ReactiveLifecycle.Expired : ReactiveLifecycle.NeverExpired;
			return true;
		}
		
		// Backend is not reactive or force non-reactive.
		if (_nonReactive)
		{
			reactiveLifecycle = default;
			return false;
		}
		
		if (_backend is WeakReference wr)
		{
			// Backend is dead.
			if (wr.Target is not { } target)
			{
				reactiveLifecycle = ReactiveLifecycle.Expired;
				return true;
			}
			
			// If target is not reactive then _nonReactive should be true.
			var backend = (IReactiveLifecycleBackend)target;
			
			// Backend is expired.
			if (backend.IsExpired(_token))
			{
				reactiveLifecycle = ReactiveLifecycle.Expired;
				return true;
			}
			
			reactiveLifecycle = ReactiveLifecycle.FromBackend(backend);
			return true;
		}
		
		if (_backend is CancellationTokenSource cts)
		{
			reactiveLifecycle = ReactiveLifecycle.FromCancellationTokenSource(cts);
			return true;
		}
		
		reactiveLifecycle = ReactiveLifecycle.FromCancellationToken((CancellationToken)_backend);
		return true;
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

			if (_backend is WeakReference wr)
			{
				if (wr.Target is not { } target)
				{
					return true;
				}

				if (target is not ILifecycleBackend backend)
				{
					return false;
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
	
	private Lifecycle(bool inlineExpired)
	{
		if (inlineExpired)
		{
			_token = LifecycleToken.InlineExpired;
		}
	}
	
	private Lifecycle(Lifecycle other, bool nonReactive)
	{
		_backend = other._backend;
		_token = other._token;
		_nonReactive = nonReactive;
	}

	private Lifecycle(CancellationTokenSource cts)
	{
		if (cts.IsCancellationRequested)
		{
			_token = LifecycleToken.InlineExpired;
		}
		else
		{
			// Keep a strong reference to cts because dead doesn't imply canceled.
			_backend = cts;
		}
	}

	private Lifecycle(CancellationToken cancellationToken)
	{
		_backend = cancellationToken;
	}

	private Lifecycle(object backend)
	{
		ensure(backend is not ValueType);
		ensure(backend is not CancellationTokenSource);
		
		if (backend is ILifecycleBackend iBackend)
		{
			if (iBackend.IsExpired(iBackend.Token))
			{
				_token = LifecycleToken.InlineExpired;
			}
			else
			{
				// Keep a weak reference to backend is enough because dead implies expired.
				_backend = new WeakReference(iBackend);
				_token = iBackend.Token;
				_nonReactive = backend is not IReactiveLifecycleBackend;
			}
		}
		else
		{
			_backend = new WeakReference(backend);
			_nonReactive = true;
		}
	}

	// WeakReference, CancellationToken, CancellationTokenSource.
	private readonly object? _backend;
	private readonly LifecycleToken _token;
	private readonly bool _nonReactive;
	
}


