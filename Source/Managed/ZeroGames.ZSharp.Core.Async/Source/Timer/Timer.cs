// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Async;

public readonly struct Timer : IEquatable<Timer>
{
	
	public bool Equals(Timer other) => _handle == other._handle;
	public override bool Equals(object? obj) => obj is Timer other && Equals(other);
	public override int32 GetHashCode() => _handle.GetHashCode();
	public static bool operator==(Timer lhs, Timer rhs) => lhs.Equals(rhs);
	public static bool operator!=(Timer lhs, Timer rhs) => !lhs.Equals(rhs);

	public void Unregister() => _owner?.Unregister(this);
	public void Suspend() => _owner?.Suspend(this);
	public void Resume() => _owner?.Resume(this);
	
	public ITimerScheduler? Owner => _owner;
	public bool IsValid => _owner?.IsValidTimer(this) ?? false;

	internal Timer(ITimerScheduler owner, uint64 handle)
	{
		_owner = owner;
		_handle = handle;
	}

	private readonly ITimerScheduler? _owner;
	private readonly uint64 _handle;
	
}


