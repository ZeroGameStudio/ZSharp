﻿// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;

namespace ZeroGames.ZSharp.Core.Async;

public readonly partial struct ReactiveLifecycle : IEquatable<ReactiveLifecycle>
{

	public bool Equals(ReactiveLifecycle other) => _underlyingLifecycle == other._underlyingLifecycle && _tokenSnapshot == other._tokenSnapshot;
	public override bool Equals(object? obj) => obj is ReactiveLifecycle other && Equals(other);
	public override int32 GetHashCode() => _underlyingLifecycle?.GetHashCode() ?? 0;
	public static bool operator==(ReactiveLifecycle lhs, ReactiveLifecycle rhs) => lhs.Equals(rhs);
	public static bool operator!=(ReactiveLifecycle lhs, ReactiveLifecycle rhs) => !lhs.Equals(rhs);

	public static implicit operator Lifecycle(ReactiveLifecycle @this)
	{
		if (@this.IsExpired)
		{
			return Lifecycle.ExpiredLifecycle;
		}

		if (@this._underlyingLifecycle is null)
		{
			return default;
		}

		return new(@this._underlyingLifecycle);
	}
	
	public LifecycleExpiredRegistration RegisterOnExpired(Action<object, object?> callback, object? state)
	{
		check(IsInGameThread);

		if (IsExpired)
		{
			callback(_underlyingLifecycle ?? this, state);
			return default;
		}

		if (_underlyingLifecycle is IExplicitLifecycle explicitLifecycle)
		{
			return new(this, explicitLifecycle.RegisterOnExpired(callback, state));
		}
		else if (_underlyingLifecycle is not null)
		{
			var reactiveUnderlyingLifecycle = Unsafe.As<IReactiveUnderlyingLifecycle>(_underlyingLifecycle);
			return reactiveUnderlyingLifecycle.RegisterOnExpired(callback, state, _tokenSnapshot);
		}

		return default;
	}

	public void UnregisterOnExpired(in LifecycleExpiredRegistration registration)
	{
		check(IsInGameThread);

		if (IsExpired)
		{
			return;
		}
		
		if (_underlyingLifecycle is IExplicitLifecycle explicitLifecycle)
		{
			explicitLifecycle.UnregisterOnExpired(registration.Explicit);
		}
		else if (_underlyingLifecycle is not null)
		{
			var reactiveUnderlyingLifecycle = Unsafe.As<IReactiveUnderlyingLifecycle>(_underlyingLifecycle);
			reactiveUnderlyingLifecycle.UnregisterOnExpired(registration, _tokenSnapshot);
		}
	}

	public bool IsValidRegistration(LifecycleExpiredRegistration registration)
	{
		check(IsInGameThread);

		if (IsExpired)
		{
			return false;
		}
		
		if (_underlyingLifecycle is IExplicitLifecycle explicitLifecycle)
		{
			return registration.Explicit.IsValid;
		}
		else if (_underlyingLifecycle is not null)
		{
			var reactiveUnderlyingLifecycle = Unsafe.As<IReactiveUnderlyingLifecycle>(_underlyingLifecycle);
			return reactiveUnderlyingLifecycle.IsValidRegistration(registration);
		}

		return false;
	}

	public bool IsExpired
	{
		get
		{
			check(IsInGameThread);

			if (_underlyingLifecycle is null)
			{
				return _tokenSnapshot == _inlineExpiredToken;
			}

			if (_tokenSnapshot.IsValid)
			{
				var interfaceUnderlyingLifecycle = Unsafe.As<IUnderlyingLifecycle>(_underlyingLifecycle);
				return _tokenSnapshot != interfaceUnderlyingLifecycle.Token || interfaceUnderlyingLifecycle.IsExpired(_tokenSnapshot);
			}
			else
			{
				return Unsafe.As<IExplicitLifecycle>(_underlyingLifecycle).IsExpired;
			}
		}
	}
	
	internal ReactiveLifecycle(IReactiveUnderlyingLifecycle underlyingLifecycle)
	{
		_underlyingLifecycle = underlyingLifecycle;
		_tokenSnapshot = underlyingLifecycle.Token;
	}

	internal ReactiveLifecycle(IExplicitLifecycle underlyingLifecycle)
	{
		if (underlyingLifecycle.IsExpired)
		{
			_tokenSnapshot = _inlineExpiredToken;
		}
		else
		{
			_underlyingLifecycle = underlyingLifecycle;
		}
	}
	
	private ReactiveLifecycle(in UnderlyingLifecycleToken inlineExpiredToken)
	{
		_tokenSnapshot = _inlineExpiredToken;
	}

	private static UnderlyingLifecycleToken _inlineExpiredToken = new(0xDEAD);
	
	private readonly object? _underlyingLifecycle;
	private readonly UnderlyingLifecycleToken _tokenSnapshot;
	
}


