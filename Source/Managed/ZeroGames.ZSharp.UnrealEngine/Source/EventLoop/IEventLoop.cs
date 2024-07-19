// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.EventLoop;

public delegate void EventLoopHandler(in EventLoopArgs args);

public interface IEventLoop
{
	public static IEventLoop Get() => EventLoop.Get();
	EventLoopObserverHandle RegisterObserver(IEventLoopObserver observer);
	EventLoopObserverHandle RegisterWeakObserver(IEventLoopObserver observer);
	EventLoopObserverHandle RegisterObserver(EEventLoopEventType eventType, EventLoopHandler observer);
	EventLoopObserverHandle RegisterObserver(EEventLoopEventType eventType, Action observer);
	EventLoopObserverHandle RegisterObserver(EEventLoopEventType eventType, Action<float> observer);
	void UnregisterObserver(IEventLoopObserver observer);
	void UnregisterObserver(EventLoopObserverHandle observer);
}


