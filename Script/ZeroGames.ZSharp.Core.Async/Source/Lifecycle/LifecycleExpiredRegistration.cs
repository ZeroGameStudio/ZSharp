// Copyright Zero Games. All Rights Reserved.

using System.Threading;

namespace ZeroGames.ZSharp.Core.Async;

public readonly struct LifecycleExpiredRegistration : IEquatable<LifecycleExpiredRegistration>, IDisposable
{

	public LifecycleExpiredRegistration(IReactiveLifecycleBackend backend, uint64 handle)
	{
		_backend = backend;
		_token = backend.Token;
		_handle = handle;
	}
	
	public LifecycleExpiredRegistration(CancellationTokenRegistration cancellationRegistration)
	{
		_cancellationRegistration = cancellationRegistration;
	}

	public void Dispose()
	{
		if (_backend is not null)
		{
			_backend.UnregisterOnExpired(this, _token);
		}
		else
		{
			_cancellationRegistration.Dispose();
		}
	}

	public bool Equals(LifecycleExpiredRegistration other) => Equals(_backend, other._backend) && _token == other._token && _handle == other._handle && _cancellationRegistration == other._cancellationRegistration;
	public override bool Equals(object? obj) => obj is LifecycleExpiredRegistration other && Equals(other);
	public override int32 GetHashCode() => _handle.GetHashCode() ^ _cancellationRegistration.GetHashCode();
	public static bool operator==(LifecycleExpiredRegistration lhs, LifecycleExpiredRegistration rhs) => lhs.Equals(rhs);
	public static bool operator!=(LifecycleExpiredRegistration lhs, LifecycleExpiredRegistration rhs) => !lhs.Equals(rhs);
	
	private readonly IReactiveLifecycleBackend? _backend;
	private readonly LifecycleToken _token;
	private readonly uint64 _handle;
	
	private readonly CancellationTokenRegistration _cancellationRegistration;

}


