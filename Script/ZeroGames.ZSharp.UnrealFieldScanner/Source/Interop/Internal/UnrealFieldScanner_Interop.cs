﻿// Copyright Zero Games. All Rights Reserved.

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

			outManifest.Data = IAssemblyResolver.Create().Resolve(assemblyName) is not null ? JsonSerializer.Serialize(new ManifestBuilder(new ModelRegistry(assemblyName, moduleName, withMetadata)).Build(), new JsonSerializerOptions
			{
				Converters = { new NumberToStringConverter() }
			}) : "{}";
		}
		catch (Exception ex)
		{
			UnhandledExceptionHelper.Guard(ex, "Unhandled exception detected during scanning unreal fields!!!", null, args->FatalMessageBuffer);
		}
	}

}


