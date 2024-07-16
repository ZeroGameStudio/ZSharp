// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Core;

public class UnrealArray<T> : UnrealArrayBase, IConjugate<UnrealArray<T>>
{

	public static UnrealArray<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);

	public UnrealArray() : base(typeof(T)){}
	public UnrealArray(IntPtr unmanaged) : base(typeof(T), unmanaged){}

	public T? this[int32 index]
	{
		get => (T?)Get(index);
		set => Set(index, value);
	}
	
}


