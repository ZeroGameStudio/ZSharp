// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;
using System.Text.Json;

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
		public IntPtr FatalMessageBuffer;
	}
	
	public static unsafe void Scan(ScanArgs* args)
	{
		try
		{
			string assemblyName = new(args->AssemblyName);
			string moduleName = new(args->ModuleName);
			using InteropString outManifest = new(args->OutManifest);
			bool withMetadata = args->WithMetadata > 0;

			outManifest.Data = JsonSerializer.Serialize(new ManifestBuilder(new ModelRegistry(assemblyName, moduleName, withMetadata)).Build());
		}
		catch (Exception ex)
		{
			UnhandledExceptionHelper.Guard(ex, args->FatalMessageBuffer, "Unhandled exception detected during scanning unreal fields!!!");
		}
	}

}


