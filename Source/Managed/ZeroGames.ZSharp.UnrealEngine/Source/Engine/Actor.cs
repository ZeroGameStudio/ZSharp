﻿// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public partial class Actor
{

	public T? GetComponentByClass<T>() where T : ActorComponent
	{
		return (T?)GetComponentByClass(new(UnrealClass.FromType<T>()));
	}

	public T[] GetComponentsByClass<T>() where T : ActorComponent
	{
		UnrealArray<ActorComponent?> comps = K2_GetComponentsByClass(new(UnrealClass.FromType<T>()));
		int32 count = comps.Count;
		var res = new T[count];
		for (int32 i = 0; i < count; ++i)
		{
			var comp = (T)comps[i]!;
			res[i] = comp;
		}

		return res;
	}

	public ENetMode GetNetMode()
	{
		MasterAlcCache.GuardInvariant();
		return InternalGetNetMode();
	}

	internal void FinishSpawning()
	{
		MasterAlcCache.GuardInvariant();
		InternalFinishSpawning();
	}

	private unsafe ENetMode InternalGetNetMode()
		=> Actor_Interop.GetNetMode(ConjugateHandle.FromConjugate(this));
	
	private unsafe void InternalFinishSpawning()
		=> Actor_Interop.FinishSpawning(ConjugateHandle.FromConjugate(this));
	
}


