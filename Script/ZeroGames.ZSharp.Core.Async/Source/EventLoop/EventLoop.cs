// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;

namespace ZeroGames.ZSharp.Core.Async;

internal class EventLoop : IEventLoop
{
	
	public EventLoopRegistration Register(EEventLoopTickingGroup group, StatelessEventLoopCallback callback, Lifecycle lifecycle = default, Action<LifecycleExpiredException>? onExpired = null)
	{
		Thrower.ThrowIfNotInGameThread();
		if (lifecycle.IsExpired)
		{
			return default;
		}
		
		return InternalRegister(group, callback, null, null, lifecycle, onExpired, null);
	}

	public EventLoopRegistration Register(EEventLoopTickingGroup group, StatefulEventLoopCallback callback, object? state, Lifecycle lifecycle = default, Action<LifecycleExpiredException, object?>? onExpired = null)
	{
		Thrower.ThrowIfNotInGameThread();
		if (lifecycle.IsExpired)
		{
			return default;
		}
		
		return InternalRegister(group, null, callback, state, lifecycle, null, onExpired);
	}

	public void Unregister(EventLoopRegistration registration)
	{
		Thrower.ThrowIfNotInGameThread();
		if (registration.Owner != this)
		{
			return;
		}
		
		InternalUnregister(registration);
	}

	public bool IsValidRegistration(EventLoopRegistration registration)
	{
		Thrower.ThrowIfNotInGameThread();
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

	public static EventLoop Instance { get; private set; }
	
	public void NotifyEvent(EEventLoopTickingGroup group, float worldDeltaSeconds, float realDeltaSeconds, double worldElapsedSeconds, double realElapsedSeconds)
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
							bool @continue = true;
							rec.StatelessCallback?.Invoke(args, ref @continue);
							rec.StatefulCallback?.Invoke(args, rec.State, ref @continue);

							if (!@continue)
							{
								registry[pair.Key] = default;
							}
						}
						else
						{
							registry[pair.Key] = default;
							rec.StatelessOnExpired?.Invoke(new(rec.Lifecycle));
							rec.StatefulOnExpired?.Invoke(new(rec.Lifecycle), rec.State);
						}
					}
					catch (Exception ex)
					{
						UnhandledExceptionHelper.Guard(ex, "Unhandled exception detected in Event Loop.", LogZSharpScriptAsync);
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
	
	public void InternalUnregister(EventLoopRegistration registration)
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

	static EventLoop()
	{
		IMasterAssemblyLoadContext.RegisterUnloading(Reinitialize, 0);
		Reinitialize();
	}

	[MemberNotNull(nameof(Instance))]
	private static void Reinitialize()
	{
		Instance = new();
	}

	private static bool IsValidRec(in Rec rec) => rec.StatelessCallback is not null || rec.StatefulCallback is not null;
	private static bool IsExpiredRec(in Rec rec) => rec.Lifecycle.IsExpired;
	
	private EventLoopRegistration InternalRegister(EEventLoopTickingGroup group, StatelessEventLoopCallback? statelessCallback, StatefulEventLoopCallback? statefulCallback, object? state, Lifecycle lifecycle, Action<LifecycleExpiredException>? statelessOnExpired, Action<LifecycleExpiredException, object?>? statefulOnExpired)
	{
		return InternalRegisterTo(_notifing ? _pendingRegistry : _registry, group, statelessCallback, statefulCallback, state, lifecycle, statelessOnExpired, statefulOnExpired);
	}

	private EventLoopRegistration InternalRegisterTo(Dictionary<EEventLoopTickingGroup, Dictionary<EventLoopRegistration, Rec>> registry, EEventLoopTickingGroup group, StatelessEventLoopCallback? statelessCallback, StatefulEventLoopCallback? statefulCallback, object? state, Lifecycle lifecycle, Action<LifecycleExpiredException>? statelessOnExpired, Action<LifecycleExpiredException, object?>? statefulOnExpired)
	{
		if (!registry.TryGetValue(group, out var innerRegistry))
		{
			innerRegistry = new();
			registry[group] = innerRegistry;
		}

		EventLoopRegistration reg = new(this, ++_handle);
		innerRegistry[reg] = new(statelessCallback, statefulCallback, state, lifecycle, statelessOnExpired, statefulOnExpired);

		return reg;
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
	
	private readonly record struct Rec(StatelessEventLoopCallback? StatelessCallback, StatefulEventLoopCallback? StatefulCallback, object? State, Lifecycle Lifecycle, Action<LifecycleExpiredException>? StatelessOnExpired, Action<LifecycleExpiredException, object?>? StatefulOnExpired);
	
	private AccumulatedSeconds _worldAccumulatedSeconds;
	private AccumulatedSeconds _realAccumulatedSeconds;
	
	private Dictionary<EEventLoopTickingGroup, Dictionary<EventLoopRegistration, Rec>> _registry = new();
	private Dictionary<EEventLoopTickingGroup, Dictionary<EventLoopRegistration, Rec>> _pendingRegistry = new();

	private bool _notifing;

	private uint64 _handle;
	
}


