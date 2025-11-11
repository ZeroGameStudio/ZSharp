// Copyright Zero Games. All Rights Reserved.

using System.Threading;

namespace ZeroGames.ZSharp.Core.Async;

public readonly partial struct Lifetime
{

	public static implicit operator Lifetime(CancellationTokenSource cts) => new(cts);
	public static implicit operator Lifetime(CancellationToken cancellationToken) => new(cancellationToken);

	public Lifetime ForceNonReactive() => new(this, true);

	public bool TryToReactive(out ReactiveLifetime reactiveLifetime)
	{
		Thrower.ThrowIfNotInGameThread();

		// Never or Expired.
		if (_backend is null)
		{
			reactiveLifetime = _token.IsInlineExpired ? ReactiveLifetime.Expired : ReactiveLifetime.NeverExpired;
			return true;
		}
		
		// Backend is not reactive or force non-reactive.
		if (_nonReactive)
		{
			reactiveLifetime = default;
			return false;
		}
		
		if (_backend is WeakReference wr)
		{
			// Backend is dead.
			if (wr.Target is not { } target)
			{
				reactiveLifetime = ReactiveLifetime.Expired;
				return true;
			}
			
			// If target is not reactive then _nonReactive should be true.
			var backend = (IReactiveLifetimeBackend)target;
			
			// Backend is expired.
			if (backend.IsExpired(_token))
			{
				reactiveLifetime = ReactiveLifetime.Expired;
				return true;
			}
			
			reactiveLifetime = ReactiveLifetime.FromBackend(backend);
			return true;
		}
		
		if (_backend is CancellationTokenSource cts)
		{
			reactiveLifetime = ReactiveLifetime.FromCancellationTokenSource(cts);
			return true;
		}
		
		reactiveLifetime = ReactiveLifetime.FromCancellationToken((CancellationToken)_backend);
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

				if (target is not ILifetimeBackend backend)
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
	
	private Lifetime(bool inlineExpired)
	{
		if (inlineExpired)
		{
			_token = LifetimeToken.InlineExpired;
		}
	}
	
	private Lifetime(Lifetime other, bool nonReactive)
	{
		_backend = other._backend;
		_token = other._token;
		_nonReactive = nonReactive;
	}

	private Lifetime(CancellationTokenSource cts)
	{
		if (cts.IsCancellationRequested)
		{
			_token = LifetimeToken.InlineExpired;
		}
		else
		{
			// Keep a strong reference to cts because dead doesn't imply canceled.
			_backend = cts;
		}
	}

	private Lifetime(CancellationToken cancellationToken)
	{
		_backend = cancellationToken;
	}

	private Lifetime(object backend)
	{
		ensure(backend is not ValueType);
		ensure(backend is not CancellationTokenSource);
		
		if (backend is ILifetimeBackend iBackend)
		{
			if (iBackend.IsExpired(iBackend.Token))
			{
				_token = LifetimeToken.InlineExpired;
			}
			else
			{
				// Keep a weak reference to backend is enough because dead implies expired.
				_backend = new WeakReference(iBackend);
				_token = iBackend.Token;
				_nonReactive = backend is not IReactiveLifetimeBackend;
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
	private readonly LifetimeToken _token;
	private readonly bool _nonReactive;
	
}


