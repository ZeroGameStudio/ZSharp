// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;

namespace ZeroGames.ZSharp.UnrealEngine.Core;

public sealed class UnrealMap<TKey, TValue> : UnrealMapBase, IConjugate<UnrealMap<TKey, TValue>>
{
	
	public static UnrealMap<TKey, TValue> BuildConjugate(IntPtr unmanaged) => new(unmanaged);

	public UnrealMap() : base(typeof(TKey), typeof(TValue)){}
	public UnrealMap(IntPtr unmanaged) : base(typeof(TKey), typeof(TValue), unmanaged){}

	public void Add(TKey key, TValue value) => base.Add(key, value);
	public void Remove(TKey key) => base.Remove(key);

	public bool Find(TKey key, [NotNullWhen(true)] out TValue? value)
	{
		if (base.Find(key, out var valueObj))
		{
			value = (TValue)valueObj;
			return true;
		}
		else
		{
			value = default;
			return false;
		}
	}

}


