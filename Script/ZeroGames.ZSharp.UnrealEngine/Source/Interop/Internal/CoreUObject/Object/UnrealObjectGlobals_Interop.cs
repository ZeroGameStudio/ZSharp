// Copyright Zero Games. All Rights Reserved.

#pragma warning disable CS0649 // Field is never assigned to, and will always have its default value

namespace ZeroGames.ZSharp.UnrealEngine;

internal static unsafe class UnrealObjectGlobals_Interop
{
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle, char*, ConjugateHandle> NewObject;
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle, char*, ConjugateHandle> LoadObject;
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle, char*, ConjugateHandle, ConjugateHandle> DuplicateObject;
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle, char*, uint8, ConjugateHandle> FindObject;
	public static delegate* unmanaged<char*, ConjugateHandle> LowLevelFindObject;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle> GetTransientPackage;
}


