// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

internal static class CoreLog
{
	
	public static void Fatal(object message) => InternalLog(1, message);
	public static void Error(object message) => InternalLog(2, message);
	public static void Warning(object message) => InternalLog(3, message);
	public static void Display(object message) => InternalLog(4, message);
	public static void Log(object message) => InternalLog(5, message);
	public static void Verbose(object message) => InternalLog(6, message);
	public static void VeryVerbose(object message) => InternalLog(7, message);

	private static void InternalLog(uint8 verbosity, object message)
	{
		unsafe
		{
			fixed (char* messageBuffer = message.ToString())
			{
				CoreLog_Interop.Log(verbosity, messageBuffer);
			}
		}
	}
	
}


