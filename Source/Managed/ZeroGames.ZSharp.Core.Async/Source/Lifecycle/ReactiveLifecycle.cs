// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;
using System.Threading;

namespace ZeroGames.ZSharp.Core.Async;

public readonly partial struct ReactiveLifecycle : IEquatable<ReactiveLifecycle>
{

	public bool Equals(ReactiveLifecycle other) => _underlyingLifecycle == other._underlyingLifecycle && _tokenSnapshot == other._tokenSnapshot;
	public override bool Equals(object? obj) => obj is ReactiveLifecycle other && Equals(other);
	public override int32 GetHashCode() => _underlyingLifecycle?.GetHashCode() ?? 0;
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

		if (@this._underlyingLifecycle is null)
		{
			return Lifecycle.NeverExpired;
		}

		return new(@this._underlyingLifecycle);
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

		if (_underlyingLifecycle is IReactiveUnderlyingLifecycle reactiveUnderlyingLifecycle)
		{
			return reactiveUnderlyingLifecycle.RegisterOnExpired(callback, _tokenSnapshot);
		}
		else if (_underlyingLifecycle is CancellationTokenSource cts)
		{
			return new(this, cts.Token.RegisterWithoutCaptureExecutionContext(callback));
		}
		else
		{
			return new(this, ((CancellationToken)_underlyingLifecycle!).RegisterWithoutCaptureExecutionContext(callback));
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

		if (_underlyingLifecycle is IReactiveUnderlyingLifecycle reactiveUnderlyingLifecycle)
		{
			return reactiveUnderlyingLifecycle.RegisterOnExpired(callback, state, _tokenSnapshot);
		}
		else if (_underlyingLifecycle is CancellationTokenSource cts)
		{
			return new(this, cts.Token.RegisterWithoutCaptureExecutionContext(callback, state));
		}
		else
		{
			return new(this, ((CancellationToken)_underlyingLifecycle!).RegisterWithoutCaptureExecutionContext(callback, state));
		}
	}

	public bool IsExpired
	{
		get
		{
			Thrower.ThrowIfNotInGameThread();

			if (_underlyingLifecycle is null)
			{
				return _tokenSnapshot.IsInlineExpired;
			}

			if (_tokenSnapshot.IsValid)
			{
				var interfaceUnderlyingLifecycle = Unsafe.As<IUnderlyingLifecycle>(_underlyingLifecycle);
				return _tokenSnapshot != interfaceUnderlyingLifecycle.Token || interfaceUnderlyingLifecycle.IsExpired(_tokenSnapshot);
			}
			else if (_underlyingLifecycle is CancellationTokenSource cts)
			{
				return cts.IsCancellationRequested;
			}
			else
			{
				return ((CancellationToken)_underlyingLifecycle!).IsCancellationRequested;
			}
		}
	}
	
	internal ReactiveLifecycle(object underlyingLifecycle)
	{
		if (underlyingLifecycle is IReactiveUnderlyingLifecycle interfaceUnderlyingLifecycle)
		{
			if (interfaceUnderlyingLifecycle.IsExpired(interfaceUnderlyingLifecycle.Token))
			{
				_tokenSnapshot = UnderlyingLifecycleToken.InlineExpired;
			}
			else
			{
				_underlyingLifecycle = interfaceUnderlyingLifecycle;
				_tokenSnapshot = interfaceUnderlyingLifecycle.Token;
			}
		}
		else if (underlyingLifecycle is CancellationToken cancellationToken)
		{
			if (cancellationToken.IsCancellationRequested)
			{
				_tokenSnapshot = UnderlyingLifecycleToken.InlineExpired;
			}
			else if (cancellationToken.CanBeCanceled)
			{
				_underlyingLifecycle = cancellationToken;
			}
		}
		else if (underlyingLifecycle is CancellationTokenSource cts)
		{
			if (cts.IsCancellationRequested)
			{
				_tokenSnapshot = UnderlyingLifecycleToken.InlineExpired;
			}
			else
			{
				_underlyingLifecycle = cts;
			}
		}
	}

	internal ReactiveLifecycle(bool inlineExpired)
	{
		if (inlineExpired)
		{
			_tokenSnapshot = UnderlyingLifecycleToken.InlineExpired;
		}
	}

	// IReactiveUnderlyingLifecycle, CancellationToken or CancellationTokenSource
	private readonly object? _underlyingLifecycle;
	private readonly UnderlyingLifecycleToken _tokenSnapshot;
	
}


