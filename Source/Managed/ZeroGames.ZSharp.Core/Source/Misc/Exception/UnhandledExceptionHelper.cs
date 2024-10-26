// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;

namespace ZeroGames.ZSharp.Core;

public static class UnhandledExceptionHelper
{

	public static void Guard(Exception exception) => Guard(exception, default, null);
	public static void Guard(Exception exception, IntPtr fatalMessageBuffer) => Guard(exception, fatalMessageBuffer, null);
	public static void Guard(Exception exception, string? messageHeader) => Guard(exception, default, messageHeader);
	public static void Guard(Exception exception, IntPtr fatalMessageBuffer, string? messageHeader)
	{
		string? fatalMessage = null;
		for (Exception? currentEx = exception; currentEx is not null; currentEx = currentEx.InnerException)
		{
			if (currentEx is FatalException fatal)
			{
				fatalMessage = fatal.Message;
				break;
			}
		}

		if (!CoreSettings.TreatManagedFatalAsError && fatalMessageBuffer != default)
		{
			using InteropString message = new(fatalMessageBuffer);
			message.Data = string.Join(Environment.NewLine, messageHeader ?? "Managed Fatal Error!!!", fatalMessage, exception);
		}
		else
		{
			UE_ERROR(LogZSharpScriptCore, string.Join(Environment.NewLine, messageHeader ?? "Unhandled exception detected.", fatalMessage, exception));
		}

		Debugger.Break();
	}
	
}


