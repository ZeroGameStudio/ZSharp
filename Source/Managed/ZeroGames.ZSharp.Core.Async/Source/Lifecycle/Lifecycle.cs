// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;
using System.Threading;

namespace ZeroGames.ZSharp.Core.Async;

public readonly partial struct Lifecycle : IEquatable<Lifecycle>
{

	public bool Equals(Lifecycle other) => _underlyingLifecycle == other._underlyingLifecycle && _tokenSnapshot == other._tokenSnapshot && _nonReactive == other._nonReactive;
	public override bool Equals(object? obj) => obj is Lifecycle other && Equals(other);
	public override int32 GetHashCode() => _underlyingLifecycle?.GetHashCode() ?? 0;
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

		if (_underlyingLifecycle is null)
		{
			reactiveLifecycle = new(_tokenSnapshot.IsInlineExpired);
			return true;
		}

		if (_underlyingLifecycle is WeakReference)
		{
			reactiveLifecycle = default;
			return false;
		}

		if (_tokenSnapshot.IsValid)
		{
			if (_underlyingLifecycle is IReactiveUnderlyingLifecycle reactiveUnderlyingLifecycle)
			{
				reactiveLifecycle = new(reactiveUnderlyingLifecycle);
				return true;
			}
			else
			{
				reactiveLifecycle = default;
				return false;
			}
		}
		else if (_underlyingLifecycle is CancellationTokenSource cts)
		{
			reactiveLifecycle = cts.Token;
			return true;
		}
		else
		{
			reactiveLifecycle = (CancellationToken)_underlyingLifecycle!;
			return true;
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

			if (_underlyingLifecycle is WeakReference wr)
			{
				return !wr.IsAlive;
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
	
	internal Lifecycle(in Lifecycle other, bool nonReactive)
	{
		_underlyingLifecycle = other._underlyingLifecycle;
		_tokenSnapshot = other._tokenSnapshot;
		_nonReactive = nonReactive;
	}

	internal Lifecycle(object underlyingLifecycle)
	{
		if (underlyingLifecycle is IUnderlyingLifecycle interfaceUnderlyingLifecycle)
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
		else if (underlyingLifecycle is not ValueType)
		{
			_underlyingLifecycle = new WeakReference(underlyingLifecycle);
			_nonReactive = true;
		}
	}

	internal Lifecycle(bool inlineExpired)
	{
		if (inlineExpired)
		{
			_tokenSnapshot = UnderlyingLifecycleToken.InlineExpired;
		}
	}

	// IUnderlyingLifecycle, CancellationToken, CancellationTokenSource or any other reference type.
	private readonly object? _underlyingLifecycle;
	private readonly UnderlyingLifecycleToken _tokenSnapshot;
	private readonly bool _nonReactive;
	
}


