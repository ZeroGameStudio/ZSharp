// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public class LazyObjectPtr<T> : LazyObjectPtrBase, IConjugate<LazyObjectPtr<T>> where T : UnrealObject
{
	
	public static LazyObjectPtr<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);

	public LazyObjectPtr() : base(typeof(T)){}
	public LazyObjectPtr(IntPtr unmanaged) : base(typeof(T), unmanaged){}
	
}


