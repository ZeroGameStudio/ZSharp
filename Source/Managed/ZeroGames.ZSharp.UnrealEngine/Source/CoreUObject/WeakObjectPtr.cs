// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public class WeakObjectPtr<T> : WeakObjectPtrBase, IConjugate<WeakObjectPtr<T>> where T : UnrealObject
{
	
	public static WeakObjectPtr<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);

	public WeakObjectPtr() : base(typeof(T)){}
	public WeakObjectPtr(IntPtr unmanaged) : base(typeof(T), unmanaged){}
	
}


