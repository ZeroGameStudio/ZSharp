// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public partial class AActor
{

	public UActorComponent AddComponent(UClass @class)
	{
		MasterAlcCache.GuardInvariant();
		return InternalAddComponent(@class, false);
	}

	public T AddComponent<T>() where T : UActorComponent, IStaticClass
		=> (T)AddComponent(T.StaticClass);

	public UActorComponent AddComponent(UClass @class, Action<UActorComponent> initialize)
	{
		MasterAlcCache.GuardInvariant();
		
		UActorComponent component = InternalAddComponent(@class, true);

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

	public T AddComponent<T>(Action<T> initialize) where T : UActorComponent, IStaticClass
	{
		MasterAlcCache.GuardInvariant();
		
		var component = (T)InternalAddComponent(T.StaticClass, true);

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
	
	public UActorComponent AddComponent<TState>(UClass @class, Action<UActorComponent, TState> initialize, TState state)
	{
		MasterAlcCache.GuardInvariant();
		
		UActorComponent component = InternalAddComponent(@class, true);

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
	
	public T AddComponent<T, TState>(Action<T, TState> initialize, TState state) where T : UActorComponent, IStaticClass
	{
		MasterAlcCache.GuardInvariant();
		
		var component = (T)InternalAddComponent(T.StaticClass, true);

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
	
	public async ZeroTask<UActorComponent> AddComponentAsync(UClass @class, Func<UActorComponent, Lifecycle, ZeroTask> initializeAsync)
	{
		MasterAlcCache.GuardInvariant();
		
		UActorComponent component = InternalAddComponent(@class, true);

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

	public async ZeroTask<T> AddComponentAsync<T>(Func<T, Lifecycle, ZeroTask> initializeAsync) where T : UActorComponent, IStaticClass
	{
		MasterAlcCache.GuardInvariant();
		
		var component = (T)InternalAddComponent(T.StaticClass, true);

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
	
	public async ZeroTask<UActorComponent> AddComponentAsync<TState>(UClass @class, Func<UActorComponent, TState, Lifecycle, ZeroTask> initializeAsync, TState state)
	{
		MasterAlcCache.GuardInvariant();
		
		UActorComponent component = InternalAddComponent(@class, true);

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
	
	public async ZeroTask<T> AddComponentAsync<T, TState>(Func<T, TState, Lifecycle, ZeroTask> initializeAsync, TState state) where T : UActorComponent, IStaticClass
	{
		MasterAlcCache.GuardInvariant();
		
		var component = (T)InternalAddComponent(T.StaticClass, true);

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

	public T GetOrAddComponent<T>() where T : UActorComponent
		=> GetComponent<T>() ?? AddComponent<T>();

	public T? GetComponent<T>() where T : UActorComponent
	{
		return (T?)GetComponentByClass(new(UClass.FromType<T>()));
	}

	public T[] GetComponents<T>() where T : UActorComponent
	{
		TArray<UActorComponent?> comps = K2_GetComponentsByClass(new(UClass.FromType<T>()));
		int32 count = comps.Count;
		var res = new T[count];
		for (int32 i = 0; i < count; ++i)
		{
			var comp = (T)comps[i]!;
			res[i] = comp;
		}

		return res;
	}

	public void GetComponents<T>(ICollection<T> result) where T : UActorComponent
	{
		TArray<UActorComponent?> comps = K2_GetComponentsByClass(new(UClass.FromType<T>()));
		foreach (UActorComponent? comp in comps)
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

	private unsafe UActorComponent InternalAddComponent(UClass @class, bool defer)
		=> Actor_Interop.AddComponent(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(@class), Convert.ToByte(defer)).GetTargetChecked<UActorComponent>();
	
	private unsafe void InternalFinishAddComponent(UActorComponent component)
		=> Actor_Interop.FinishAddComponent(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(component));
	
	private unsafe ENetMode InternalGetNetMode()
		=> Actor_Interop.GetNetMode(ConjugateHandle.FromConjugate(this));

}


