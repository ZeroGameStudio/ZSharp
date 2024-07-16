// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public class SoftObjectPtr<T> : SoftObjectPtrBase, IConjugate<SoftObjectPtr<T>> where T : UnrealObject?
{
	
	public static SoftObjectPtr<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);

	public SoftObjectPtr() : base(typeof(T)){}
	public SoftObjectPtr(IntPtr unmanaged) : base(typeof(T), unmanaged){}
	
}


