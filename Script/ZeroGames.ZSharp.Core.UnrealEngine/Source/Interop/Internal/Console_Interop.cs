// Copyright Zero Games. All Rights Reserved.

#pragma warning disable CS0649 // Field is never assigned to, and will always have its default value

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core.UnrealEngine;

internal static unsafe class Console_Interop
{

	[UnmanagedCallersOnly]
	public static void HandleExecuteCommand(char* nameBuffer, IntPtr argsAddr)
	{
		try
		{
			using InteropStringArray argsBuffer = new(argsAddr);
		
			ConsoleManager.HandleExecuteCommand(new(nameBuffer), argsBuffer.ToStringArray());
		}
		catch (Exception ex)
		{
			UnhandledExceptionHelper.Guard(ex);
		}
	}

	[UnmanagedCallersOnly]
	public static void HandleVariableChanged(char* nameBuffer)
	{
		try
		{
			ConsoleManager.HandleVariableChanged(new(nameBuffer));
		}
		catch (Exception ex)
		{
			UnhandledExceptionHelper.Guard(ex);
		}
	}

	public static delegate* unmanaged<char*, uint8> TryExecuteCommand;
	public static delegate* unmanaged<char*, IntPtr, uint8> TryGetValue;
	public static delegate* unmanaged<char*, char*, uint8> TrySetValue;
	public static delegate* unmanaged<char*, uint8> TryRegisterOnChanged;
	public static delegate* unmanaged<char*, uint8> TryUnregisterOnChanged;

	public static delegate* unmanaged<char*, char*, uint8> TryRegisterCommand;
	public static delegate* unmanaged<char*, char*, char*, uint8> TryRegisterVariable;
	public static delegate* unmanaged<char*, uint8> TryUnregisterObject;

}


