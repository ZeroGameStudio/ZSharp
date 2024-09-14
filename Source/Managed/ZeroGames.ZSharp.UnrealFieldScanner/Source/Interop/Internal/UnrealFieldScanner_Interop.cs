// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

internal static class UnrealFieldScanner_Interop
{
	
	[StructLayout(LayoutKind.Sequential)]
	public unsafe struct ScanArgs
	{
		public char* AssemblyName;
		public char* ModuleName;
		public IntPtr OutManifest;
		public uint8 WithMetadata;
	}
	
	public static unsafe void Scan(ScanArgs* args)
	{
		try
		{
			string assemblyName = new(args->AssemblyName);
			string moduleName = new(args->ModuleName);
			InteropString outManifest = new(args->OutManifest);
			bool withMetadata = args->WithMetadata > 0;
			
			using UnrealFieldScanner scanner = new(assemblyName, moduleName, withMetadata);
			outManifest.Data = scanner.Scan();
		}
		catch (Exception ex)
		{
			Logger.Fatal(ex);
		}
	}

}


