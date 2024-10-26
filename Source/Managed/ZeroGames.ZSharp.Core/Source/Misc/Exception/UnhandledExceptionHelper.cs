// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;

namespace ZeroGames.ZSharp.Core;

public static class UnhandledExceptionHelper
{

	public static void Guard(Exception exception) => Guard(exception, default);
	
	public static void Guard(Exception exception, IntPtr fatalMessageBuffer)
	{
		if (fatalMessageBuffer != default)
		{
			for (Exception? currentEx = exception; currentEx is not null; currentEx = currentEx.InnerException)
			{
				if (currentEx is FatalException fatal)
				{
					using InteropString message = new(fatalMessageBuffer);
					message.Data = string.Join(Environment.NewLine, "Managed Fatal Error!!!", fatal.Message, exception);
					break;
				}
			}
		}
		else
		{
			UE_ERROR(LogZSharpScriptCore, $"Unhandled Exception Detected.\n{exception}");
		}

		Debugger.Break();
	}
	
}


