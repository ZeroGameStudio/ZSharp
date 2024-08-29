// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public partial class Actor
{

	public T? GetComponentByClass<T>() where T : ActorComponent
	{
		return (T?)GetComponentByClass(new(UnrealObjectGlobals.GetClass<T>()));
	}

	public T[] GetComponentsByClass<T>() where T : ActorComponent
	{
		UnrealArray<ActorComponent?> comps = K2_GetComponentsByClass(new(UnrealObjectGlobals.GetClass<T>()));
		int32 count = comps.Count;
		var res = new T[count];
		for (int32 i = 0; i < count; ++i)
		{
			var comp = Unsafe.As<T>(comps[i]!);
			res[i] = comp;
		}

		return res;
	}
	
}


