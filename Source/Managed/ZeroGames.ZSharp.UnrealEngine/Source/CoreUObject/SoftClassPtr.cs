// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public class SoftClassPtr<T> : SoftClassPtrBase, IConjugate<SoftClassPtr<T>> where T : IUnrealObject
{
	
	public static SoftClassPtr<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);

	public SoftClassPtr() : base(typeof(T)){}
	public SoftClassPtr(IntPtr unmanaged) : base(typeof(T), unmanaged){}
	
}


