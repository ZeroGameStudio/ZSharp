// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public class StrongObjectPtr<T> : StrongObjectPtrBase, IConjugate<StrongObjectPtr<T>> where T : UnrealObject?
{
	
	public static StrongObjectPtr<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);

	public StrongObjectPtr() : base(typeof(T)){}
	public StrongObjectPtr(IntPtr unmanaged) : base(typeof(T), unmanaged){}
	
}


