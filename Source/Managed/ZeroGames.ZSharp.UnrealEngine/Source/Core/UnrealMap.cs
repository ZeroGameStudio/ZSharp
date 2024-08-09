// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Core;

public class UnrealMap<TKey, TValue> : UnrealMapBase, IConjugate<UnrealMap<TKey, TValue>>
{
	
	public static UnrealMap<TKey, TValue> BuildConjugate(IntPtr unmanaged) => new(unmanaged);

	public UnrealMap() : base(typeof(TKey), typeof(TValue)){}
	public UnrealMap(IntPtr unmanaged) : base(typeof(TKey), typeof(TValue), unmanaged){}
	
}


