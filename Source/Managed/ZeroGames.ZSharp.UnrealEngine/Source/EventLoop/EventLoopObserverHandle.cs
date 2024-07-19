// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.EventLoop;

public readonly struct EventLoopObserverHandle()
{

	internal EventLoopObserverHandle(uint64 inner) : this()
	{
		_inner = inner;
	}
	
	public override int32 GetHashCode()
	{
		return _inner.GetHashCode();
	}

	public bool IsValid => _inner > 0;

	private readonly uint64 _inner;

}


