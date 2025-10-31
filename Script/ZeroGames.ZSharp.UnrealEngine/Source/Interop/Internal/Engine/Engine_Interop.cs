// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

#pragma warning disable CS0649 // Field is never assigned to, and will always have its default value

namespace ZeroGames.ZSharp.UnrealEngine;

internal static unsafe class Engine_Interop
{
	[UnmanagedCallersOnly]
	public static void PostEngineInit()
	{
		try
		{
			UEngine.InternalPostEngineInit();
		}
		catch (Exception ex)
		{
			UnhandledExceptionHelper.Guard(ex);
		}
	}
	
	public static delegate* unmanaged<uint8> IsInitialized;
	public static delegate* unmanaged<ConjugateHandle> GetEngine;
}


