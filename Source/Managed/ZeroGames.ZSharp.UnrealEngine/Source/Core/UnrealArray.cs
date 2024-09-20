// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Core;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
public sealed class UnrealArray<T> : UnrealArrayBase, IConjugate<UnrealArray<T>>
{

	public static UnrealArray<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);

	public UnrealArray() : base(typeof(T)){}
	public UnrealArray(IntPtr unmanaged) : base(typeof(T), unmanaged){}

	public T this[int32 index]
	{
		get => (T)Get(index)!;
		set => Set(index, value);
	}
	
}


