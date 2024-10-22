// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

internal class EventLoop : IEventLoop
{

	public EventLoopRegistration Register(EEventLoopTickingGroup group, EventLoopCallback callback, object? state, Lifecycle lifecycle = default, Action<LifecycleExpiredException>? onExpired = null)
	{
		ThreadHelper.ValidateGameThread();
		if (lifecycle.IsExpired)
		{
			return default;
		}
		
		return InternalRegister(group, callback, state, lifecycle, onExpired);
	}

	public void Unregister(EventLoopRegistration registration)
	{
		ThreadHelper.ValidateGameThread();
		if (registration.Owner != this)
		{
			return;
		}
		
		InternalUnregister(registration);
	}

	public void UnregisterAll(Lifecycle lifecycle)
	{
		ThreadHelper.ValidateGameThread();
		InternalUnregisterAll(lifecycle);
	}

	public bool IsValidRegistration(EventLoopRegistration registration)
	{
		ThreadHelper.ValidateGameThread();
		return InternalIsValidRegistration(registration);
	}

	private bool InternalIsValidRegistration(EventLoopRegistration registration)
	{
		static bool Traverse(Dictionary<EEventLoopTickingGroup, Dictionary<EventLoopRegistration, Rec>> registry, EventLoopRegistration registration, ref bool value)
		{
			foreach (var pair in registry)
			{
				foreach (var p in pair.Value)
				{
					if (p.Key == registration)
					{
						value = IsValidRec(p.Value);
						return true;
					}
				}
			}

			return false;
		}

		if (registration.Owner != this)
		{
			return false;
		}

		bool valid = false;
		if (_notifing && Traverse(_pendingRegistry, registration, ref valid))
		{
			return valid;
		}

		Traverse(_registry, registration, ref valid);
		return valid;
	}

	internal static EventLoop Instance { get; } = new();
	
	internal void NotifyEvent(EEventLoopTickingGroup group, float worldDeltaSeconds, float realDeltaSeconds, double worldElapsedSeconds, double realElapsedSeconds)
	{
		_notifing = true;
			
		if (group == EEventLoopTickingGroup.PreWorldTick)
		{
			_worldAccumulatedSeconds += worldDeltaSeconds;
			_realAccumulatedSeconds += realDeltaSeconds;
		}
				
		EventLoopArgs args = new()
		{
			TickingGroup = group,
			WorldDeltaSeconds = worldDeltaSeconds,
			RealDeltaSeconds = realDeltaSeconds,
			WorldElapsedSeconds = worldElapsedSeconds,
			RealElapsedSeconds = realElapsedSeconds,
			WorldAccumulatedSeconds = _worldAccumulatedSeconds.Seconds,
			RealAccumulatedSeconds = _realAccumulatedSeconds.Seconds,
		};
				
		if (_registry.TryGetValue(group, out var registry))
		{
			EventLoopRegistration stale = default;
			foreach (var pair in registry)
			{
				Rec rec = pair.Value;
				if (IsValidRec(rec))
				{
					try
					{
						if (!IsExpiredRec(rec))
						{
							rec.Callback!(args, rec.State);
						}
						else
						{
							registry[pair.Key] = default;
							rec.OnExpired?.Invoke(new(rec.Lifecycle));
						}
					}
					catch (Exception ex)
					{
						UE_ERROR(LogTemp, $"Unhandled Exception Detected in Event Loop.\n{ex}");
					}
				}
					
				// Rec may have modified by callback/onExpired.
				rec = pair.Value;
				if (!IsValidRec(rec))
				{
					stale = pair.Key;
				}
			}

			// Clear only one stale registration because we run very frequently.
			if (stale != default)
			{
				registry.Remove(stale);
			}

			FlushPendingRegistry();
		}

		_notifing = false;
	}
	
	internal void InternalUnregister(EventLoopRegistration registration)
	{
		static bool Traverse(Dictionary<EEventLoopTickingGroup, Dictionary<EventLoopRegistration, Rec>> registry, EventLoopRegistration reg)
		{
			foreach (var pair in registry)
			{
				var innerRegistry = pair.Value;
				if (innerRegistry.ContainsKey(reg))
				{
					innerRegistry[reg] = default;
					return true;
				}
			}

			return false;
		}

		if (!_notifing || !Traverse(_pendingRegistry, registration))
		{
			Traverse(_registry, registration);
		}
	}

	private static bool IsValidRec(in Rec rec) => rec.Callback is not null;
	private static bool IsExpiredRec(in Rec rec) => rec.Lifecycle.IsExpired;
	
	private EventLoopRegistration InternalRegister(EEventLoopTickingGroup group, EventLoopCallback callback, object? state, Lifecycle lifecycle, Action<LifecycleExpiredException>? onExpired)
	{
		return InternalRegisterTo(_notifing ? _pendingRegistry : _registry, group, callback, state, lifecycle, onExpired);
	}

	private EventLoopRegistration InternalRegisterTo(Dictionary<EEventLoopTickingGroup, Dictionary<EventLoopRegistration, Rec>> registry, EEventLoopTickingGroup group, EventLoopCallback callback, object? state, Lifecycle lifecycle, Action<LifecycleExpiredException>? onExpired)
	{
		if (!registry.TryGetValue(group, out var innerRegistry))
		{
			innerRegistry = new();
			registry[group] = innerRegistry;
		}

		EventLoopRegistration reg = new(this, ++_handle);
		innerRegistry[reg] = new(callback, state, lifecycle, onExpired);

		return reg;
	}

	private void InternalUnregisterAll(Lifecycle lifecycle)
	{
		static void Traverse(Dictionary<EEventLoopTickingGroup, Dictionary<EventLoopRegistration, Rec>> registry, Lifecycle lifecycle)
		{
			foreach (var pair in registry)
			{
				var innerRegistry = pair.Value;
				foreach (var innerPair in innerRegistry)
				{
					if (innerPair.Value.Lifecycle == lifecycle)
					{
						innerRegistry[innerPair.Key] = default;
					}
				}
			}
		}

		
		if (_notifing)
		{
			Traverse(_pendingRegistry, lifecycle);
		}

		Traverse(_registry, lifecycle);
	}

	private void FlushPendingRegistry()
	{
		foreach (var pair in _pendingRegistry)
		{
			var innerRegistry = pair.Value;
			foreach (var innerPair in innerRegistry)
			{
				Rec rec = innerPair.Value;
				if (IsValidRec(rec))
				{
					// This method is only used by NotifyEvent which already acquires lock, so here we don't lock again.
					RegisterPending(pair.Key, innerPair.Key, rec);
				}
			}
			innerRegistry.Clear();
		}
	}
	
	private void RegisterPending(EEventLoopTickingGroup group, EventLoopRegistration registration, in Rec rec)
	{
		if (!_registry.TryGetValue(group, out var innerRegistry))
		{
			innerRegistry = new();
			_registry[group] = innerRegistry;
		}

		innerRegistry[registration] = rec;
	}
	
	private readonly record struct Rec(EventLoopCallback? Callback, object? State, Lifecycle Lifecycle, Action<LifecycleExpiredException>? OnExpired);
	
	private AccumulatedSeconds _worldAccumulatedSeconds;
	private AccumulatedSeconds _realAccumulatedSeconds;
	
	private Dictionary<EEventLoopTickingGroup, Dictionary<EventLoopRegistration, Rec>> _registry = new();
	private Dictionary<EEventLoopTickingGroup, Dictionary<EventLoopRegistration, Rec>> _pendingRegistry = new();

	private bool _notifing;

	private uint64 _handle;
	
}


