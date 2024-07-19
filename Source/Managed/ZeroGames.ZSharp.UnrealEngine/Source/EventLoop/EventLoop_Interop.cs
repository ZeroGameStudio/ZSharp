// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine.EventLoop;

internal static class EventLoop_Interop
{

	[UnmanagedCallersOnly]
	public static void NotifyEvent(EEventLoopEventType eventType, float worldDeltaTime, float realDeltaTime, double worldElapsedTime, double realElapsedTime) => Uncaught.FatalIfUncaught(() =>
	{
		EventLoop.Get().NotifyEvent(eventType, worldDeltaTime, realDeltaTime, worldElapsedTime, realElapsedTime);
	});

}


