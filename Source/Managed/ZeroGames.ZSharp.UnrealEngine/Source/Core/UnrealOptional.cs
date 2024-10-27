// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;

namespace ZeroGames.ZSharp.UnrealEngine.Core;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
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
			verify(Get(out var valueObj));
			return (T)valueObj;
		}
		set => Set(value);
	}
	
}


