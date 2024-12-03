// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine;

internal static unsafe class UnrealArray_Interop
{
	public static delegate* unmanaged<ConjugateHandle, int32, ZCallBufferSlot, void> Insert;
	public static delegate* unmanaged<ConjugateHandle, ZCallBufferSlot, uint8> Remove;
	public static delegate* unmanaged<ConjugateHandle, int32, void> RemoveAt;
	public static delegate* unmanaged<ConjugateHandle, void> Clear;
	public static delegate* unmanaged<ConjugateHandle, ZCallBufferSlot, int32> IndexOf;
	public static delegate* unmanaged<ConjugateHandle, int32, ref ZCallBufferSlot, void> Get;
	public static delegate* unmanaged<ConjugateHandle, int32, ZCallBufferSlot, void> Set;
	public static delegate* unmanaged<ConjugateHandle, int32> Count;
}


