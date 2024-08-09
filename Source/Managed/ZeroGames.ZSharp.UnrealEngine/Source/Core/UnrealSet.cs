// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Core;

public class UnrealSet<T> : UnrealSetBase, IConjugate<UnrealSet<T>>
{
	
	public static UnrealSet<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);

	public UnrealSet() : base(typeof(T)){}
	public UnrealSet(IntPtr unmanaged) : base(typeof(T), unmanaged){}
	
}


