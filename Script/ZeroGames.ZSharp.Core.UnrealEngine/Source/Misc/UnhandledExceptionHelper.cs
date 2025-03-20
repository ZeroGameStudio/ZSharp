// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;

namespace ZeroGames.ZSharp.Core.UnrealEngine;

public static class UnhandledExceptionHelper
{

	public static void Guard(Exception exception) => Guard(exception, IntPtr.Zero);
	public static void Guard(Exception exception, IntPtr fatalMessageBuffer) => Guard(exception, null, null, fatalMessageBuffer);
	public static void Guard(Exception exception, string? messageHeader) => Guard(exception, messageHeader, null);
	public static void Guard(Exception exception, string? messageHeader, string? logCategory) => Guard(exception, messageHeader, logCategory, IntPtr.Zero);
	public static void Guard(Exception exception, string? messageHeader, string? logCategory, IntPtr fatalMessageBuffer)
	{
		string? fatalMessage = null;
		for (Exception? currentEx = exception; currentEx is not null; currentEx = currentEx.InnerException)
		{
			if (currentEx is OperationCanceledException)
			{
				return;
			}
			
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

		string finalMessage = fatalMessage is not null ? $"Managed Fatal Error!!! {fatalMessage}{Environment.NewLine}{exception}" : $"Unhandled exception detected.{Environment.NewLine}{exception}";
		if (fatalMessage is not null && !CoreSettings.TreatManagedFatalAsError && fatalMessageBuffer != default)
		{
			using InteropString message = new(fatalMessageBuffer);
			message.Data = finalMessage;
		}
		else
		{
			UE_ERROR(logCategory ?? LogZSharpScript, finalMessage);
		}

		Debugger.Break();
	}
	
}


