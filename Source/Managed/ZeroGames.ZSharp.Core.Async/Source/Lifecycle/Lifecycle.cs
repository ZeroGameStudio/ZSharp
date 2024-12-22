// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;
using System.Threading;

namespace ZeroGames.ZSharp.Core.Async;

public readonly partial struct Lifecycle : IEquatable<Lifecycle>
{

	public bool Equals(Lifecycle other) => _backend == other._backend && _token == other._token && _nonReactive == other._nonReactive;
	public override bool Equals(object? obj) => obj is Lifecycle other && Equals(other);
	public override int32 GetHashCode() => _backend?.GetHashCode() ?? 0;
	public static bool operator==(Lifecycle lhs, Lifecycle rhs) => lhs.Equals(rhs);
	public static bool operator!=(Lifecycle lhs, Lifecycle rhs) => !lhs.Equals(rhs);

	public static implicit operator Lifecycle(CancellationToken cancellationToken) => new(cancellationToken);

	public Lifecycle ForceNonReactive() => new(this, true);

	public bool TryToReactive(out ReactiveLifecycle reactiveLifecycle)
	{
		Thrower.ThrowIfNotInGameThread();

		if (_nonReactive)
		{
			reactiveLifecycle = default;
			return false;
		}

		if (_backend is null)
		{
			reactiveLifecycle = new(_token.IsInlineExpired);
			return true;
		}

		if (_backend is WeakReference)
		{
			reactiveLifecycle = default;
			return false;
		}

		if (_token.IsValid)
		{
			if (_backend is IReactiveLifecycleBackend reactiveBackend)
			{
				reactiveLifecycle = new(reactiveBackend);
				return true;
			}
			else
			{
				reactiveLifecycle = default;
				return false;
			}
		}
		else if (_backend is CancellationTokenSource cts)
		{
			reactiveLifecycle = cts.Token;
			return true;
		}
		else
		{
			reactiveLifecycle = (CancellationToken)_backend!;
			return true;
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

			if (_backend is WeakReference wr)
			{
				return !wr.IsAlive;
			}

			if (_token.IsValid)
			{
				var backend = Unsafe.As<ILifecycleBackend>(_backend);
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
	
	internal Lifecycle(in Lifecycle other, bool nonReactive)
	{
		_backend = other._backend;
		_token = other._token;
		_nonReactive = nonReactive;
	}

	internal Lifecycle(object backend)
	{
		if (backend is ILifecycleBackend iBackend)
		{
			if (iBackend.IsExpired(iBackend.Token))
			{
				_token = LifecycleToken.InlineExpired;
			}
			else
			{
				_backend = iBackend;
				_token = iBackend.Token;
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
		else if (backend is not ValueType)
		{
			_backend = new WeakReference(backend);
			_nonReactive = true;
		}
	}

	internal Lifecycle(bool inlineExpired)
	{
		if (inlineExpired)
		{
			_token = LifecycleToken.InlineExpired;
		}
	}

	// ILifecycleBackend, CancellationToken, CancellationTokenSource or any other reference type.
	private readonly object? _backend;
	private readonly LifecycleToken _token;
	private readonly bool _nonReactive;
	
}


