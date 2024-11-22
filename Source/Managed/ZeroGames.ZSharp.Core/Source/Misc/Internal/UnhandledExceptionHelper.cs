// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;

namespace ZeroGames.ZSharp.Core;

internal static class UnhandledExceptionHelper
{

	public static void Guard(Exception exception) => Guard(exception, IntPtr.Zero);
	public static void Guard(Exception exception, IntPtr fatalMessageBuffer) => Guard(exception, null, fatalMessageBuffer);
	public static void Guard(Exception exception, string? messageHeader) => Guard(exception, messageHeader, IntPtr.Zero);
	public static void Guard(Exception exception, string? messageHeader, IntPtr fatalMessageBuffer)
	{
		string? fatalMessage = null;
		for (Exception? currentEx = exception; currentEx is not null; currentEx = currentEx.InnerException)
		{
			if (CoreSettings.SuppressAlcUnloadedException && currentEx is AlcUnloadedException)
			{
				return;
			}
			
			if (currentEx is FatalException fatal)
			{
				fatalMessage = fatal.Message;
				break;
			}
		}
		
		string finalMessage = fatalMessage is not null ? $"{messageHeader ?? "Managed Fatal Error!!!"} {fatalMessage}{Environment.NewLine}{exception}" : $"{messageHeader ?? "Unhandled exception detected."}{Environment.NewLine}{exception}";
		if (fatalMessage is not null && !CoreSettings.TreatManagedFatalAsError && fatalMessageBuffer != default)
		{
			using InteropString message = new(fatalMessageBuffer);
			message.Data = finalMessage;
		}
		else
		{
			CoreLog.Error(finalMessage);
		}

		Debugger.Break();
	}
	
}


