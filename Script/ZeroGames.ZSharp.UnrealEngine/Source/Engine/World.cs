// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public enum ESpawnActorNameMode : uint8
{
	Required_Fatal,
	Required_ErrorAndReturnNull,
	Required_ReturnNull,
	Requested,
}

public readonly struct ActorSpawnParameters
{
	public string? Name { get; init; }
	public AActor? Template { get; init; }
	public AActor? Owner { get; init; }
	public APawn? Instigator { get; init; }
	public ULevel? OverrideLevel { get; init; }
	public ESpawnActorCollisionHandlingMethod? SpawnCollisionHandlingOverride { get; init; }
	public ESpawnActorScaleMethod? TransformScaleMethod { get; init; }
	public ESpawnActorNameMode? NameMode { get; init; }
}

public partial class UWorld
{

	public static event Action<UWorld>? OnInitialized;
	public static event Action<UWorld>? OnTearingDown;
	public static event Action<string>? PreLoadMap;
	public static event Action<UWorld>? PostLoadMap;
	
	public T? GetSubsystem<T>() where T : UWorldSubsystem, IStaticClass => USubsystemBlueprintLibrary.GetWorldSubsystem(this, T.StaticClass) as T;

	#region SpawnActor
	
	public T? SpawnActor<T>(UClass @class, FTransform? transform, in ActorSpawnParameters spawnParameters = default) where T : AActor, IStaticClass
	{
		if (!@class.IsChildOf<T>())
		{
			throw new ArgumentOutOfRangeException(nameof(@class));
		}

		if (InternalSpawnActor(@class, transform, spawnParameters, false, false) is T actor)
		{
			return actor;
		}

		return null;
	}
	
	public AActor? SpawnActor(UClass @class, FTransform? transform, in ActorSpawnParameters spawnParameters = default)
		=> SpawnActor<AActor>(@class, transform, spawnParameters);

	public T? SpawnActor<T>(UClass @class, FVector? location, FRotator? rotation, in ActorSpawnParameters spawnParameters = default) where T : AActor, IStaticClass
		=> SpawnActor<T>(@class, new() { Translation = location, Rotation = rotation?.Quat }, spawnParameters);
	
	public T? SpawnActor<T>(FTransform? transform, in ActorSpawnParameters spawnParameters = default) where T : AActor, IStaticClass
		=> SpawnActor<T>(T.StaticClass, transform, spawnParameters);
	
	public T? SpawnActor<T>(FVector? location, FRotator? rotation, in ActorSpawnParameters spawnParameters = default) where T : AActor, IStaticClass
		=> SpawnActor<T>(T.StaticClass, location, rotation, spawnParameters);
	
	#endregion
	
	#region SpawnActorAbsolute
	
	public T? SpawnActorAbsolute<T>(UClass @class, FTransform? transform, in ActorSpawnParameters spawnParameters = default) where T : AActor, IStaticClass
	{
		if (!@class.IsChildOf<T>())
		{
			throw new ArgumentOutOfRangeException(nameof(@class));
		}
		
		return (T?)SpawnActorAbsolute(@class, transform, spawnParameters);
	}
	
	public AActor? SpawnActorAbsolute(UClass @class, FTransform? transform, in ActorSpawnParameters spawnParameters = default)
		=> SpawnActorAbsolute<AActor>(@class, transform, spawnParameters);

	public T? SpawnActorAbsolute<T>(UClass @class, FVector? location, FRotator? rotation, in ActorSpawnParameters spawnParameters = default) where T : AActor, IStaticClass
		=> SpawnActorAbsolute<T>(@class, new() { Translation = location, Rotation = rotation?.Quat }, spawnParameters);

	public T? SpawnActorAbsolute<T>(FTransform? transform, in ActorSpawnParameters spawnParameters = default) where T : AActor, IStaticClass
		=> SpawnActorAbsolute<T>(T.StaticClass, transform, spawnParameters);
	
	public T? SpawnActorAbsolute<T>(FVector? location, FRotator? rotation, in ActorSpawnParameters spawnParameters = default) where T : AActor, IStaticClass
		=> SpawnActorAbsolute<T>(T.StaticClass, location, rotation, spawnParameters);
	
	#endregion
	
	#region SpawnActorDeferred

	public T? SpawnActorDeferred<T>(UClass @class, FTransform? transform, Func<T, FTransform?> initialize, in ActorSpawnParameters spawnParameters = default) where T : AActor, IStaticClass
	{
		MasterAlcCache.GuardInvariant();
		
		if (!@class.IsChildOf<T>())
		{
			throw new ArgumentOutOfRangeException(nameof(@class));
		}
		
		if (InternalSpawnActor(@class, transform, spawnParameters, false, true) is T actor)
		{
			FTransform? userTransform = null;
			try
			{
				userTransform = initialize(actor);
			}
			catch (Exception ex)
			{
				throw new ActorInitializationFailedException(actor, ex);
			}
			finally
			{
				actor.FinishSpawning(userTransform ?? transform);
			}
			
			return actor;
		}

		return null;
	}

	public AActor? SpawnActorDeferred(UClass @class, FTransform? transform, Func<AActor, FTransform?> initialize, in ActorSpawnParameters spawnParameters = default)
		=> SpawnActorDeferred<AActor>(@class, transform, initialize, spawnParameters);

	public T? SpawnActorDeferred<T>(UClass @class, FVector? location, FRotator? rotation, Func<T, FTransform?> initialize, in ActorSpawnParameters spawnParameters = default) where T : AActor, IStaticClass
		=> SpawnActorDeferred<T>(@class, new() { Translation = location, Rotation = rotation?.Quat }, initialize, spawnParameters);

	public T? SpawnActorDeferred<T>(FTransform? transform, Func<T, FTransform?> initialize, in ActorSpawnParameters spawnParameters = default) where T : AActor, IStaticClass
		=> SpawnActorDeferred<T>(T.StaticClass, transform, initialize, spawnParameters);
	
	public T? SpawnActorDeferred<T>(FVector? location, FRotator? rotation, Func<T, FTransform?> initialize, in ActorSpawnParameters spawnParameters = default) where T : AActor, IStaticClass
		=> SpawnActorDeferred<T>(T.StaticClass, location, rotation, initialize, spawnParameters);
	
	public T? SpawnActorDeferred<T, TState>(UClass @class, FTransform? transform, Func<T, TState, FTransform?> initialize, TState state, in ActorSpawnParameters spawnParameters = default) where T : AActor, IStaticClass
	{
		MasterAlcCache.GuardInvariant();
		
		if (!@class.IsChildOf<T>())
		{
			throw new ArgumentOutOfRangeException(nameof(@class));
		}
		
		if (InternalSpawnActor(@class, transform, spawnParameters, false, true) is T actor)
		{
			FTransform? userTransform = null;
			try
			{
				userTransform = initialize(actor, state);
			}
			catch (Exception ex)
			{
				throw new ActorInitializationFailedException(actor, ex);
			}
			finally
			{
				actor.FinishSpawning(userTransform ?? transform);
			}
			
			return actor;
		}

		return null;
	}
	
	public AActor? SpawnActorDeferred<TState>(UClass @class, FTransform? transform, Func<AActor, TState, FTransform?> initialize, TState state, in ActorSpawnParameters spawnParameters = default)
		=> SpawnActorDeferred<AActor, TState>(@class, transform, initialize, state, spawnParameters);

	public T? SpawnActorDeferred<T, TState>(UClass @class, FVector? location, FRotator? rotation, Func<T, TState, FTransform?> initialize, TState state, in ActorSpawnParameters spawnParameters = default) where T : AActor, IStaticClass
		=> SpawnActorDeferred<T, TState>(@class, new() { Translation = location, Rotation = rotation?.Quat }, initialize, state, spawnParameters);
	
	public T? SpawnActorDeferred<T, TState>(FTransform? transform, Func<T, TState, FTransform?> initialize, TState state, in ActorSpawnParameters spawnParameters = default) where T : AActor, IStaticClass
		=> SpawnActorDeferred<T, TState>(T.StaticClass, transform, initialize, state, spawnParameters);
	
	public T? SpawnActorDeferred<T, TState>(FVector? location, FRotator? rotation, Func<T, TState, FTransform?> initialize, TState state, in ActorSpawnParameters spawnParameters = default) where T : AActor, IStaticClass
		=> SpawnActorDeferred<T, TState>(T.StaticClass, location, rotation, initialize, state, spawnParameters);
	
	#endregion
	
	#region SpawnActorDeferredAsync

	public ZeroTask<T?> SpawnActorDeferredAsync<T>(UClass @class, FTransform? transform, Func<T, Lifetime, ZeroTask<FTransform?>> initializeAsync, in ActorSpawnParameters spawnParameters = default) where T : AActor, IStaticClass
	{
		MasterAlcCache.GuardInvariant();
		
		if (!@class.IsChildOf<T>())
		{
			throw new ArgumentOutOfRangeException(nameof(@class));
		}

		return InternalSpawnActor(@class, transform, spawnParameters, false, true) is T actor ? InternalInitializeActorAsync(actor, transform, initializeAsync) : ZeroTask.FromResult<T?>(null);
	}
	
	public ZeroTask<AActor?> SpawnActorDeferredAsync(UClass @class, FTransform? transform, Func<AActor, Lifetime, ZeroTask<FTransform?>> initializeAsync, in ActorSpawnParameters spawnParameters = default)
		=> SpawnActorDeferredAsync<AActor>(@class, transform, initializeAsync, spawnParameters);

	public ZeroTask<T?> SpawnActorDeferredAsync<T>(UClass @class, FVector? location, FRotator? rotation, Func<T, Lifetime, ZeroTask<FTransform?>> initializeAsync, in ActorSpawnParameters spawnParameters = default) where T : AActor, IStaticClass
		=> SpawnActorDeferredAsync<T>(@class, new() { Translation = location, Rotation = rotation?.Quat }, initializeAsync, spawnParameters);

	public ZeroTask<T?> SpawnActorDeferredAsync<T>(FTransform? transform, Func<T, Lifetime, ZeroTask<FTransform?>> initializeAsync, in ActorSpawnParameters spawnParameters = default) where T : AActor, IStaticClass
		=> SpawnActorDeferredAsync<T>(T.StaticClass, transform, initializeAsync, spawnParameters);

	public ZeroTask<T?> SpawnActorDeferredAsync<T>(FVector? location, FRotator? rotation, Func<T, Lifetime, ZeroTask<FTransform?>> initializeAsync, in ActorSpawnParameters spawnParameters = default) where T : AActor, IStaticClass
		=> SpawnActorDeferredAsync<T>(T.StaticClass, new() { Translation = location, Rotation = rotation?.Quat }, initializeAsync, spawnParameters);

	public ZeroTask<T?> SpawnActorDeferredAsync<T, TState>(UClass @class, FTransform? transform, Func<T, TState, Lifetime, ZeroTask<FTransform?>> initializeAsync, TState state, in ActorSpawnParameters spawnParameters = default) where T : AActor, IStaticClass
	{
		MasterAlcCache.GuardInvariant();
		
		if (!@class.IsChildOf<T>())
		{
			throw new ArgumentOutOfRangeException(nameof(@class));
		}
		
		return InternalSpawnActor(@class, transform, spawnParameters, false, true) is T actor ? InternalInitializeActorAsync(actor, transform, initializeAsync, state) : ZeroTask.FromResult<T?>(null);
	}
	
	public ZeroTask<AActor?> SpawnActorDeferredAsync<TState>(UClass @class, FTransform? transform, Func<AActor, TState, Lifetime, ZeroTask<FTransform?>> initializeAsync, TState state, in ActorSpawnParameters spawnParameters = default)
		=> SpawnActorDeferredAsync<AActor, TState>(@class, transform, initializeAsync, state, spawnParameters);

	public ZeroTask<T?> SpawnActorDeferredAsync<T, TState>(UClass @class, FVector? location, FRotator? rotation, Func<T, TState, Lifetime, ZeroTask<FTransform?>> initializeAsync, TState state, in ActorSpawnParameters spawnParameters = default) where T : AActor, IStaticClass
		=> SpawnActorDeferredAsync<T, TState>(@class, new() { Translation = location, Rotation = rotation?.Quat }, initializeAsync, state, spawnParameters);
	
	public ZeroTask<T?> SpawnActorDeferredAsync<T, TState>(FTransform? transform, Func<T, TState, Lifetime, ZeroTask<FTransform?>> initializeAsync, TState state, in ActorSpawnParameters spawnParameters = default) where T : AActor, IStaticClass
		=> SpawnActorDeferredAsync<T, TState>(T.StaticClass, transform, initializeAsync, state, spawnParameters);

	public ZeroTask<T?> SpawnActorDeferredAsync<T, TState>(FVector? location, FRotator? rotation, Func<T, TState, Lifetime, ZeroTask<FTransform?>> initializeAsync, TState state, in ActorSpawnParameters spawnParameters = default) where T : AActor, IStaticClass
		=> SpawnActorDeferredAsync<T, TState>(T.StaticClass, new() { Translation = location, Rotation = rotation?.Quat }, initializeAsync, state, spawnParameters);

	#endregion

	public EWorldType GetWorldType()
	{
		MasterAlcCache.GuardInvariant();
		return InternalGetWorldType();
	}

	public ENetMode GetNetMode()
	{
		MasterAlcCache.GuardInvariant();
		return InternalGetNetMode();
	}

	internal static void InternalNotifyWorldInitialized(ConjugateHandle world)
		=> OnInitialized?.Invoke(world.GetTargetChecked<UWorld>());
	
	internal static void InternalNotifyWorldTearingDown(ConjugateHandle world)
		=> OnTearingDown?.Invoke(world.GetTargetChecked<UWorld>());
	
	internal static void InternalPreLoadMap(string map)
		=> PreLoadMap?.Invoke(map);
	
	internal static void InternalPostLoadMap(ConjugateHandle world)
		=> PostLoadMap?.Invoke(world.GetTargetChecked<UWorld>());

	private unsafe AActor? InternalSpawnActor(UClass @class, FTransform? transform, in ActorSpawnParameters spawnParameters, bool absolute, bool deferred)
	{
		fixed (char* nameBuffer = spawnParameters.Name)
		{
			return World_Interop.SpawnActor
			(
				ConjugateHandle.FromConjugate(this), 
				ConjugateHandle.FromConjugate(@class),
				ConjugateHandle.FromConjugate(transform),
				nameBuffer,
				ConjugateHandle.FromConjugate(spawnParameters.Template),
				ConjugateHandle.FromConjugate(spawnParameters.Owner),
				ConjugateHandle.FromConjugate(spawnParameters.Instigator),
				ConjugateHandle.FromConjugate(spawnParameters.OverrideLevel),
				spawnParameters.SpawnCollisionHandlingOverride ?? ESpawnActorCollisionHandlingMethod.Undefined,
				spawnParameters.TransformScaleMethod ?? ESpawnActorScaleMethod.MultiplyWithRoot,
				spawnParameters.NameMode ?? ESpawnActorNameMode.Required_Fatal,
				Convert.ToByte(absolute),
				Convert.ToByte(deferred)
			).GetTarget<AActor>();
		}
	}

	private async ZeroTask<T?> InternalInitializeActorAsync<T>(T actor, FTransform? transform, Func<T, Lifetime, ZeroTask<FTransform?>> initializeAsync) where T : AActor, IStaticClass
	{
		FTransform? userTransform = null;
		try
		{
			userTransform = await initializeAsync(actor, Lifetime);
		}
		catch (Exception ex) when (ex is not OperationCanceledException) // FIXME: How to deal with this?
		{
			throw new ActorInitializationFailedException(actor, ex);
		}
		finally
		{
			if (__IsValid)
			{
				actor.FinishSpawning(userTransform ?? transform);
			}
		}
			
		return actor;
	}

	private async ZeroTask<T?> InternalInitializeActorAsync<T, TState>(T actor, FTransform? transform, Func<T, TState, Lifetime, ZeroTask<FTransform?>> initializeAsync, TState state) where T : AActor, IStaticClass
	{
		FTransform? userTransform = null;
		try
		{
			userTransform = await initializeAsync(actor, state, Lifetime);
		}
		catch (Exception ex) when (ex is not OperationCanceledException) // FIXME: How to deal with this?
		{
			throw new ActorInitializationFailedException(actor, ex);
		}
		finally
		{
			if (__IsValid)
			{
				actor.FinishSpawning(userTransform ?? transform);
			}
		}
			
		return actor;
	}
	
	private unsafe EWorldType InternalGetWorldType()
		=> World_Interop.GetWorldType(ConjugateHandle.FromConjugate(this));
	
	private unsafe ENetMode InternalGetNetMode()
		=> World_Interop.GetNetMode(ConjugateHandle.FromConjugate(this));
	
}


