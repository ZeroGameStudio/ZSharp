// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public enum ESpawnActorNameMode : uint8
{
	Required_Fatal,
	Required_ErrorAndReturnNull,
	Required_ReturnNull,
	Requested,
}

public struct ActorSpawnParameters()
{
	public string? Name { get; set; }
	public Actor? Template { get; set; }
	public Actor? Owner { get; set; }
	public Pawn? Instigator { get; set; }
	public Level? OverrideLevel { get; set; }
	public ESpawnActorCollisionHandlingMethod SpawnCollisionHandlingOverride { get; set; }
	public ESpawnActorScaleMethod TransformScaleMethod { get; set; } = ESpawnActorScaleMethod.MultiplyWithRoot;
	public ESpawnActorNameMode NameMode { get; set; }
}

public partial class World
{
	
	public T? GetSubsystem<T>() where T : WorldSubsystem, IStaticClass => SubsystemBlueprintLibrary.GetWorldSubsystem(this, T.StaticClass) as T;

	#region SpawnActor
	
	public T? SpawnActor<T>(UnrealClass @class, Transform? transform, in ActorSpawnParameters spawnParameters = default) where T : Actor, IStaticClass
	{
		if (!@class.IsChildOf<T>())
		{
			throw new ArgumentOutOfRangeException(nameof(@class));
		}
		
		return (T?)SpawnActor(@class, transform, spawnParameters);
	}
	
	public Actor? SpawnActor(UnrealClass @class, Transform? transform, in ActorSpawnParameters spawnParameters = default)
		=> SpawnActor<Actor>(@class, transform, spawnParameters);

	public T? SpawnActor<T>(UnrealClass @class, Vector? location, Rotator? rotation, in ActorSpawnParameters spawnParameters = default) where T : Actor, IStaticClass
		=> SpawnActor<T>(@class, new() { Translation = location, Rotation = rotation?.ToQuat() }, spawnParameters);
	
	public T? SpawnActor<T>(Transform? transform, in ActorSpawnParameters spawnParameters = default) where T : Actor, IStaticClass
		=> SpawnActor<T>(T.StaticClass, transform, spawnParameters);
	
	public T? SpawnActor<T>(Vector? location, Rotator? rotation, in ActorSpawnParameters spawnParameters = default) where T : Actor, IStaticClass
		=> SpawnActor<T>(T.StaticClass, location, rotation, spawnParameters);
	
	#endregion
	
	#region SpawnActorAbsolute
	
	public T? SpawnActorAbsolute<T>(UnrealClass @class, Transform? transform, in ActorSpawnParameters spawnParameters = default) where T : Actor, IStaticClass
	{
		if (!@class.IsChildOf<T>())
		{
			throw new ArgumentOutOfRangeException(nameof(@class));
		}
		
		return (T?)SpawnActorAbsolute(@class, transform, spawnParameters);
	}
	
	public Actor? SpawnActorAbsolute(UnrealClass @class, Transform? transform, in ActorSpawnParameters spawnParameters = default)
		=> SpawnActorAbsolute<Actor>(@class, transform, spawnParameters);

	public T? SpawnActorAbsolute<T>(UnrealClass @class, Vector? location, Rotator? rotation, in ActorSpawnParameters spawnParameters = default) where T : Actor, IStaticClass
		=> SpawnActorAbsolute<T>(@class, new() { Translation = location, Rotation = rotation?.ToQuat() }, spawnParameters);

	public T? SpawnActorAbsolute<T>(Transform? transform, in ActorSpawnParameters spawnParameters = default) where T : Actor, IStaticClass
		=> SpawnActorAbsolute<T>(T.StaticClass, transform, spawnParameters);
	
	public T? SpawnActorAbsolute<T>(Vector? location, Rotator? rotation, in ActorSpawnParameters spawnParameters = default) where T : Actor, IStaticClass
		=> SpawnActorAbsolute<T>(T.StaticClass, location, rotation, spawnParameters);
	
	#endregion
	
	#region SpawnActorDeferred

	public T? SpawnActorDeferred<T>(UnrealClass @class, Transform? transform, Action<T> initialize, in ActorSpawnParameters spawnParameters = default) where T : Actor, IStaticClass
	{
		MasterAlcCache.GuardInvariant();
		
		if (!@class.IsChildOf<T>())
		{
			throw new ArgumentOutOfRangeException(nameof(@class));
		}
		
		if (InternalSpawnActor(@class, transform, spawnParameters, false, true) is T actor)
		{
			try
			{
				initialize(actor);
			}
			catch (Exception ex)
			{
				throw new ActorInitializationFailedException(actor, ex);
			}
			finally
			{
				actor.FinishSpawning();
			}
			
			return actor;
		}

		return null;
	}

	public Actor? SpawnActorDeferred(UnrealClass @class, Transform? transform, Action<Actor> initialize, in ActorSpawnParameters spawnParameters = default)
		=> SpawnActorDeferred<Actor>(@class, transform, initialize, spawnParameters);

	public T? SpawnActorDeferred<T>(UnrealClass @class, Vector? location, Rotator? rotation, Action<T> initialize, in ActorSpawnParameters spawnParameters = default) where T : Actor, IStaticClass
		=> SpawnActorDeferred<T>(@class, new() { Translation = location, Rotation = rotation?.ToQuat() }, initialize, spawnParameters);

	public T? SpawnActorDeferred<T>(Transform? transform, Action<T> initialize, in ActorSpawnParameters spawnParameters = default) where T : Actor, IStaticClass
		=> SpawnActorDeferred<T>(T.StaticClass, transform, initialize, spawnParameters);
	
	public T? SpawnActorDeferred<T>(Vector? location, Rotator? rotation, Action<T> initialize, in ActorSpawnParameters spawnParameters = default) where T : Actor, IStaticClass
		=> SpawnActorDeferred<T>(T.StaticClass, location, rotation, initialize, spawnParameters);
	
	public T? SpawnActorDeferred<T, TState>(UnrealClass @class, Transform? transform, Action<T, TState> initialize, TState state, in ActorSpawnParameters spawnParameters = default) where T : Actor, IStaticClass
	{
		MasterAlcCache.GuardInvariant();
		
		if (!@class.IsChildOf<T>())
		{
			throw new ArgumentOutOfRangeException(nameof(@class));
		}
		
		if (InternalSpawnActor(@class, transform, spawnParameters, false, true) is T actor)
		{
			try
			{
				initialize(actor, state);
			}
			catch (Exception ex)
			{
				throw new ActorInitializationFailedException(actor, ex);
			}
			finally
			{
				actor.FinishSpawning();
			}
			
			return actor;
		}

		return null;
	}
	
	public Actor? SpawnActorDeferred<TState>(UnrealClass @class, Transform? transform, Action<Actor, TState> initialize, TState state, in ActorSpawnParameters spawnParameters = default)
		=> SpawnActorDeferred<Actor, TState>(@class, transform, initialize, state, spawnParameters);

	public T? SpawnActorDeferred<T, TState>(UnrealClass @class, Vector? location, Rotator? rotation, Action<T, TState> initialize, TState state, in ActorSpawnParameters spawnParameters = default) where T : Actor, IStaticClass
		=> SpawnActorDeferred<T, TState>(@class, new() { Translation = location, Rotation = rotation?.ToQuat() }, initialize, state, spawnParameters);
	
	public T? SpawnActorDeferred<T, TState>(Transform? transform, Action<T, TState> initialize, TState state, in ActorSpawnParameters spawnParameters = default) where T : Actor, IStaticClass
		=> SpawnActorDeferred<T, TState>(T.StaticClass, transform, initialize, state, spawnParameters);
	
	public T? SpawnActorDeferred<T, TState>(Vector? location, Rotator? rotation, Action<T, TState> initialize, TState state, in ActorSpawnParameters spawnParameters = default) where T : Actor, IStaticClass
		=> SpawnActorDeferred<T, TState>(T.StaticClass, location, rotation, initialize, state, spawnParameters);
	
	#endregion
	
	#region SpawnActorDeferredAsync

	public ZeroTask<T?> SpawnActorDeferredAsync<T>(UnrealClass @class, Transform? transform, Func<T, ZeroTask> initializeAsync, in ActorSpawnParameters spawnParameters = default) where T : Actor, IStaticClass
	{
		MasterAlcCache.GuardInvariant();
		
		if (!@class.IsChildOf<T>())
		{
			throw new ArgumentOutOfRangeException(nameof(@class));
		}

		return InternalSpawnActor(@class, transform, spawnParameters, false, true) is T actor ? InternalInitializeActorAsync(actor, initializeAsync) : ZeroTask.FromResult<T?>(null);
	}
	
	public ZeroTask<Actor?> SpawnActorDeferredAsync(UnrealClass @class, Transform? transform, Func<Actor, ZeroTask> initializeAsync, in ActorSpawnParameters spawnParameters = default)
		=> SpawnActorDeferredAsync<Actor>(@class, transform, initializeAsync, spawnParameters);

	public ZeroTask<T?> SpawnActorDeferredAsync<T>(UnrealClass @class, Vector? location, Rotator? rotation, Func<T, ZeroTask> initializeAsync, in ActorSpawnParameters spawnParameters = default) where T : Actor, IStaticClass
		=> SpawnActorDeferredAsync<T>(@class, new() { Translation = location, Rotation = rotation?.ToQuat() }, initializeAsync, spawnParameters);

	public ZeroTask<T?> SpawnActorDeferredAsync<T>(Transform? transform, Func<T, ZeroTask> initializeAsync, in ActorSpawnParameters spawnParameters = default) where T : Actor, IStaticClass
		=> SpawnActorDeferredAsync<T>(T.StaticClass, transform, initializeAsync, spawnParameters);

	public ZeroTask<T?> SpawnActorDeferredAsync<T>(Vector? location, Rotator? rotation, Func<T, ZeroTask> initializeAsync, in ActorSpawnParameters spawnParameters = default) where T : Actor, IStaticClass
		=> SpawnActorDeferredAsync<T>(T.StaticClass, new() { Translation = location, Rotation = rotation?.ToQuat() }, initializeAsync, spawnParameters);

	public ZeroTask<T?> SpawnActorDeferredAsync<T, TState>(UnrealClass @class, Transform? transform, Func<T, TState, ZeroTask> initializeAsync, TState state, in ActorSpawnParameters spawnParameters = default) where T : Actor, IStaticClass
	{
		MasterAlcCache.GuardInvariant();
		
		if (!@class.IsChildOf<T>())
		{
			throw new ArgumentOutOfRangeException(nameof(@class));
		}
		
		return InternalSpawnActor(@class, transform, spawnParameters, false, true) is T actor ? InternalInitializeActorAsync(actor, initializeAsync, state) : ZeroTask.FromResult<T?>(null);
	}
	
	public ZeroTask<Actor?> SpawnActorDeferredAsync<TState>(UnrealClass @class, Transform? transform, Func<Actor, TState, ZeroTask> initializeAsync, TState state, in ActorSpawnParameters spawnParameters = default)
		=> SpawnActorDeferredAsync<Actor, TState>(@class, transform, initializeAsync, state, spawnParameters);

	public ZeroTask<T?> SpawnActorDeferredAsync<T, TState>(UnrealClass @class, Vector? location, Rotator? rotation, Func<T, TState, ZeroTask> initializeAsync, TState state, in ActorSpawnParameters spawnParameters = default) where T : Actor, IStaticClass
		=> SpawnActorDeferredAsync<T, TState>(@class, new() { Translation = location, Rotation = rotation?.ToQuat() }, initializeAsync, state, spawnParameters);
	
	public ZeroTask<T?> SpawnActorDeferredAsync<T, TState>(Transform? transform, Func<T, TState, ZeroTask> initializeAsync, TState state, in ActorSpawnParameters spawnParameters = default) where T : Actor, IStaticClass
		=> SpawnActorDeferredAsync<T, TState>(T.StaticClass, transform, initializeAsync, state, spawnParameters);

	public ZeroTask<T?> SpawnActorDeferredAsync<T, TState>(Vector? location, Rotator? rotation, Func<T, TState, ZeroTask> initializeAsync, TState state, in ActorSpawnParameters spawnParameters = default) where T : Actor, IStaticClass
		=> SpawnActorDeferredAsync<T, TState>(T.StaticClass, new() { Translation = location, Rotation = rotation?.ToQuat() }, initializeAsync, state, spawnParameters);

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

	private unsafe Actor? InternalSpawnActor(UnrealClass @class, Transform? transform, in ActorSpawnParameters spawnParameters, bool absolute, bool deferred)
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
				spawnParameters.SpawnCollisionHandlingOverride,
				spawnParameters.TransformScaleMethod,
				spawnParameters.NameMode,
				Convert.ToByte(absolute),
				Convert.ToByte(deferred)
			).GetTarget<Actor>();
		}
	}

	private async ZeroTask<T?> InternalInitializeActorAsync<T>(T actor, Func<T, ZeroTask> initializeAsync) where T : Actor, IStaticClass
	{
		try
		{
			await initializeAsync(actor);
		}
		catch (Exception ex)
		{
			throw new ActorInitializationFailedException(actor, ex);
		}
		finally
		{
			actor.FinishSpawning();
		}
			
		return actor;
	}

	private async ZeroTask<T?> InternalInitializeActorAsync<T, TState>(T actor, Func<T, TState, ZeroTask> initializeAsync, TState state) where T : Actor, IStaticClass
	{
		try
		{
			await initializeAsync(actor, state);
		}
		catch (Exception ex)
		{
			throw new ActorInitializationFailedException(actor, ex);
		}
		finally
		{
			actor.FinishSpawning();
		}
			
		return actor;
	}
	
	private unsafe EWorldType InternalGetWorldType()
		=> World_Interop.GetWorldType(ConjugateHandle.FromConjugate(this));
	
	private unsafe ENetMode InternalGetNetMode()
		=> World_Interop.GetNetMode(ConjugateHandle.FromConjugate(this));
	
}


