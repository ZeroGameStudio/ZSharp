// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Core;

public class UnrealOptional<T> : UnrealOptionalBase, IConjugate<UnrealOptional<T>>
{
	
	public static UnrealOptional<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);

	public UnrealOptional() : base(typeof(T)){}
	public UnrealOptional(IntPtr unmanaged) : base(typeof(T), unmanaged){}
	
}


