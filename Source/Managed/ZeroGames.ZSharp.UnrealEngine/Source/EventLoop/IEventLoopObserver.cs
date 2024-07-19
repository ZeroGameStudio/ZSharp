// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.EventLoop;

public interface IEventLoopObserver
{
	void NotifyEvent(in EventLoopArgs args);
	EEventLoopEventType EventType { get; }
	EventLoopObserverHandle Handle { get; set; } // WARNING: NEVER SET this in user code.
}


