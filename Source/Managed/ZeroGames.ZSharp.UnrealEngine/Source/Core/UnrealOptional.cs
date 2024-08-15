// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;

namespace ZeroGames.ZSharp.UnrealEngine.Core;

public sealed class UnrealOptional<T> : UnrealOptionalBase, IConjugate<UnrealOptional<T>>
{
	
	public static UnrealOptional<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);

	public UnrealOptional() : base(typeof(T)){}
	public UnrealOptional(IntPtr unmanaged) : base(typeof(T), unmanaged){}

	public bool TryGetValue([NotNullWhen(true)] out T? value)
	{
		if (Get(out var valueObj))
		{
			value = (T)valueObj;
			return true;
		}
		else
		{
			value = default;
			return false;
		}
	}

	public T Value
	{
		get
		{
			if (Get(out var valueObj))
			{
				return (T)valueObj;
			}

			throw new InvalidOperationException();
		}
		set => Set(value);
	}
	
}


