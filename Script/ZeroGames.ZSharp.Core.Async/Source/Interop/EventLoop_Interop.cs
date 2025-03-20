// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core.Async;

internal static class EventLoop_Interop
{

	[UnmanagedCallersOnly]
	public static void NotifyEvent(EEventLoopTickingGroup group, float worldDeltaSeconds, float realDeltaSeconds, double worldElapsedSeconds, double realElapsedSeconds, IntPtr fatalMessageBuffer)
	{
		try
		{
			EventLoop.Instance.NotifyEvent(group, worldDeltaSeconds, realDeltaSeconds, worldElapsedSeconds, realElapsedSeconds);
		}
		catch (Exception ex)
		{
			UnhandledExceptionHelper.Guard(ex, null, LogZSharpScriptAsync, fatalMessageBuffer);
		}
	}

}


