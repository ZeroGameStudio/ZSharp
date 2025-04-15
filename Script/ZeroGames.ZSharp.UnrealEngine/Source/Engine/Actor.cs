// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public partial class Actor
{

	public T AddComponent<T>() where T : ActorComponent
	{
		MasterAlcCache.GuardInvariant();
		return InternalAddComponent<T>(false);
	}

	public T AddComponent<T>(Action<T> initialize) where T : ActorComponent
	{
		MasterAlcCache.GuardInvariant();
		
		T component = InternalAddComponent<T>(true);

		try
		{
			initialize(component);
		}
		finally
		{
			InternalFinishAddComponent(component);
		}

		return component;
	}
	
	public T AddComponent<T, TState>(Action<T, TState> initialize, TState state) where T : ActorComponent
	{
		MasterAlcCache.GuardInvariant();
		
		T component = InternalAddComponent<T>(true);

		try
		{
			initialize(component, state);
		}
		finally
		{
			InternalFinishAddComponent(component);
		}

		return component;
	}

	public async ZeroTask<T> AddComponentAsync<T>(Func<T, Lifecycle, ZeroTask> initializeAsync) where T : ActorComponent
	{
		MasterAlcCache.GuardInvariant();
		
		T component = InternalAddComponent<T>(true);

		try
		{
			await initializeAsync(component, Lifecycle);
		}
		finally
		{
			if (!IsExpired)
			{
				InternalFinishAddComponent(component);
			}
		}
		
		return component;
	}
	
	public async ZeroTask<T> AddComponentAsync<T, TState>(Func<T, TState, Lifecycle, ZeroTask> initializeAsync, TState state) where T : ActorComponent
	{
		MasterAlcCache.GuardInvariant();
		
		T component = InternalAddComponent<T>(true);

		try
		{
			await initializeAsync(component, state, Lifecycle);
		}
		finally
		{
			if (!IsExpired)
			{
				InternalFinishAddComponent(component);
			}
		}
		
		return component;
	}

	public T GetOrAddComponent<T>() where T : ActorComponent
		=> GetComponent<T>() ?? AddComponent<T>();

	public T? GetComponent<T>() where T : ActorComponent
	{
		return (T?)GetComponentByClass(new(UnrealClass.FromType<T>()));
	}

	public T[] GetComponents<T>() where T : ActorComponent
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

	public void GetComponents<T>(ICollection<T> result) where T : ActorComponent
	{
		UnrealArray<ActorComponent?> comps = K2_GetComponentsByClass(new(UnrealClass.FromType<T>()));
		foreach (ActorComponent? comp in comps)
		{
			result.Add((T)comp!);
		}
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
	
	private unsafe void InternalFinishSpawning()
		=> Actor_Interop.FinishSpawning(ConjugateHandle.FromConjugate(this));

	private unsafe T InternalAddComponent<T>(bool defer) where T : ActorComponent
		=> Actor_Interop.AddComponent(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(UnrealClass.FromType<T>()), Convert.ToByte(defer)).GetTargetChecked<T>();
	
	private unsafe void InternalFinishAddComponent(ActorComponent component)
		=> Actor_Interop.FinishAddComponent(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(component));
	
	private unsafe ENetMode InternalGetNetMode()
		=> Actor_Interop.GetNetMode(ConjugateHandle.FromConjugate(this));

}


