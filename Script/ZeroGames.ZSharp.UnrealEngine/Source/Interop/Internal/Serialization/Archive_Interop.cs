// Copyright Zero Games. All Rights Reserved.

#pragma warning disable CS0649 // Field is never assigned to, and will always have its default value

namespace ZeroGames.ZSharp.UnrealEngine;

// We don't know actual type of archive, maybe should not use [SuppressGCTransition].
internal static unsafe class Archive_Interop
{
	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, uint8, void> SaveUInt8;
	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, uint8> LoadUInt8;

	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, uint16, void> SaveUInt16;
	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, uint16> LoadUInt16;

	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, uint32, void> SaveUInt32;
	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, uint32> LoadUInt32;

	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, uint64, void> SaveUInt64;
	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, uint64> LoadUInt64;

	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, int8, void> SaveInt8;
	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, int8> LoadInt8;

	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, int16, void> SaveInt16;
	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, int16> LoadInt16;

	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, int32, void> SaveInt32;
	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, int32> LoadInt32;

	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, int64, void> SaveInt64;
	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, int64> LoadInt64;

	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, float, void> SaveFloat;
	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, float> LoadFloat;

	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, double, void> SaveDouble;
	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, double> LoadDouble;

	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, ConjugateHandle, void> SaveString;
	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, ConjugateHandle> LoadString;

	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, ConjugateHandle, void> SaveAnsiString;
	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, ConjugateHandle> LoadAnsiString;

	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, ConjugateHandle, void> SaveUtf8String;
	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, ConjugateHandle> LoadUtf8String;

	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, ConjugateHandle, void> SaveName;
	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, ConjugateHandle> LoadName;

	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, ConjugateHandle, void> SaveText;
	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, ConjugateHandle> LoadText;

	public static delegate* unmanaged<IntPtr, ConjugateHandle, void> SaveObject;
	public static delegate* unmanaged<IntPtr, ConjugateHandle> LoadObject;

	public static delegate* unmanaged<IntPtr, ConjugateHandle, void> SaveScriptStruct;
	public static delegate* unmanaged<IntPtr, ConjugateHandle> LoadScriptStruct;
	
	public static delegate* unmanaged<IntPtr, ConjugateHandle, ConjugateHandle, out uint8, uint8> NetSaveScriptStruct;
	public static delegate* unmanaged<IntPtr, out ConjugateHandle, ConjugateHandle, out uint8, uint8> NetLoadScriptStruct;
	
	public static delegate* unmanaged<IntPtr, ConjugateHandle, void> SaveSoftObjectPath;
	public static delegate* unmanaged<IntPtr, ConjugateHandle> LoadSoftObjectPath;

	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, uint8> IsSaving;
	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, uint8> IsLoading;
	
	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, int64> Tell;
	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, int64, void> Seek;
	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, int64> GetTotalSize;
}


