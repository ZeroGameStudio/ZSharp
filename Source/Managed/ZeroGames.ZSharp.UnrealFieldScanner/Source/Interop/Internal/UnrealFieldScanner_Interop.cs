// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

internal static class UnrealFieldScanner_Interop
{
	
	[StructLayout(LayoutKind.Sequential)]
	public unsafe struct ScanArgs
	{
		public char* AssemblyName;
		public IntPtr OutManifest;
	}

	public static unsafe void Scan(ScanArgs* args)
	{
		try
		{
			string assemblyName = new(args->AssemblyName);
			InteropString outManifest = new(args->OutManifest);
			outManifest.Data = UnrealFieldScanner.Instance.Scan(assemblyName);
		}
		catch (Exception ex)
		{
			Logger.Error(ex);
		}
	}

}


