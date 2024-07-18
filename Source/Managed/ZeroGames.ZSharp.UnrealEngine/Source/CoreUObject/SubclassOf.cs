// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public class SubclassOf<T> : SubclassOfBase, IConjugate<SubclassOf<T>> where T : IUnrealObject
{
	
	public static SubclassOf<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);

	public SubclassOf() : base(typeof(T)){}
	public SubclassOf(IntPtr unmanaged) : base(typeof(T), unmanaged){}
	
}


