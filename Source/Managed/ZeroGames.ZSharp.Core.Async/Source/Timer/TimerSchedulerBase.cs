// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public abstract class TimerSchedulerBase : ITimerScheduler
{

	public TimerSchedulerBase()
	{
		ErrorHandler = DefaultErrorHandler;
	}

	public TimerSchedulerBase(Action<Exception> errorHandler) : this()
	{
		ErrorHandler = errorHandler;
	}

	public Timer Register(Action<TimeSpan, object?> callback, object? state, TimeSpan rate, bool looped = false, Lifecycle lifecycle = default, Action<LifecycleExpiredException, object?>? onExpired = null)
	{
		check(IsInGameThread);
		if (looped && rate < _minLoopRate)
		{
			rate = _minLoopRate;
		}

		return InternalRegister(callback, state, rate, looped, lifecycle, onExpired);
	}

	private Timer InternalRegister(Action<TimeSpan, object?> callback, object? state, TimeSpan rate, bool looped = false, Lifecycle lifecycle = default, Action<LifecycleExpiredException, object?>? onExpired = null)
	{
		Timer timer = new(this, ++_handle);
		TimerData data = TimerData.GetFromPool();
		data.StartTime = AccumulatedTime;
		data.Callback = callback;
		data.State = state;
		data.Rate = rate;
		data.IsLooped = looped;
		data.Lifecycle = lifecycle;
		data.OnExpired = onExpired;
		data.SuspendVersion = 0;
		_registry[timer] = data;
		TakeSnapshot(timer, data);

		return timer;
	}

	private void InternalUnregister(Timer timer)
	{
		if (_registry.Remove(timer, out var data))
		{
			data.ReturnToPool();
			return;
		}

		if (_suspendedRegistry.Remove(timer, out data))
		{
			data.ReturnToPool();
		}
	}
	
	private void InternalSuspend(Timer timer)
	{
		if (_registry.Remove(timer, out var data))
		{
			_suspendedRegistry[timer] = data;
			++data.SuspendVersion;
		}
	}
	
	private void InternalResume(Timer timer)
	{
		if (_suspendedRegistry.Remove(timer, out var data))
		{
			_registry[timer] = data;
			TakeSnapshot(timer, data);
		}
	}

	public void Unregister(Timer timer)
	{
		check(IsInGameThread);
		if (timer.Owner != this)
		{
			return;
		}
		
		InternalUnregister(timer);
	}

	public void UnregisterAll(Lifecycle lifecycle)
	{
		check(IsInGameThread);
		throw new NotSupportedException();
	}
	
	public void Suspend(Timer timer)
	{
		check(IsInGameThread);
		if (timer.Owner != this)
		{
			return;
		}
		
		InternalSuspend(timer);
	}

	public void SuspendAll(Lifecycle lifecycle)
	{
		check(IsInGameThread);
		throw new NotSupportedException();
	}

	public void Resume(Timer timer)
	{
		check(IsInGameThread);
		if (timer.Owner != this)
		{
			return;
		}
		
		InternalResume(timer);
	}

	public void ResumeAll(Lifecycle lifecycle)
	{
		check(IsInGameThread);
		throw new NotSupportedException();
	}

	public bool IsValidTimer(Timer timer)
	{
		check(IsInGameThread);
		return InternalIsValidTimer(timer);
	}

	public void Tick(float deltaSeconds)
	{
		check(IsInGameThread);
		ArgumentOutOfRangeException.ThrowIfNegativeOrZero(deltaSeconds);
		
		if (_ticking)
		{
			throw new InvalidOperationException();
		}
		
		_ticking = true;
		
		_accumulatedSeconds += deltaSeconds;
		
		BeginTick();

		while (_snapshots.TryPeek(out var snapshot, out var triggerTimeSnapshop))
		{
			TimeSpan accumulatedTime = _accumulatedSeconds.TimeSpan;
			if (triggerTimeSnapshop > accumulatedTime)
			{
				// 1. This is a min heap so there is no need to continue iterate anymore.
				// 2. We only trigger OnExpired when the timer get triggered, otherwise it's behavior will be unpredictable.
				
				break;
			}
			
			// Consume this snapshot.
			_snapshots.Dequeue();

			// Execute timer.
			Timer timer = snapshot.Item1;
			uint64 suspendVersionSnapshot = snapshot.Item2;
			bool expired = false;
			// IMPORTANT: We must query timer data every time because user code may have side effects.
			TimerData? data;
			while (_registry.TryGetValue(timer, out data) && data.SuspendVersion == suspendVersionSnapshot && data.TriggerTime <= accumulatedTime && !expired)
			{
				// Advance modifies data.StartTime so we take a snapshot.
				TimeSpan startTime = data.StartTime;
				
				// Advance first so if user calls Suspend/Resume it can work fine.
				Advance(data);
				
				try
				{
					if (!data.Lifecycle.IsExpired)
					{
						data.Callback(accumulatedTime - startTime, data.State);
					}
					else
					{
						expired = true;
						data.OnExpired?.Invoke(new(data.Lifecycle), data.State);
					}
				}
				catch (Exception ex)
				{
					try
					{
						ErrorHandler(ex);
					}
					catch (Exception innerEx)
					{
						Thrower.Fatal($"TimerScheduler.ErrorHandler throws exception itself!!!\n{innerEx}", innerEx);
					}
				}
			}

			// Timer finish execution (not removed by user, not suspend and resume in-place).
			if (data is not null && data.SuspendVersion == suspendVersionSnapshot)
			{
				if (data.IsLooped && !expired)
				{
					TakeSnapshot(timer, data);
				}
				else
				{
					Unregister(timer);
				}
			}
			
			// If we are out of budget, defer remaining task to next tick.
			if (IsOverBudget)
			{
				break;
			}
		}
		
		EndTick();

		_ticking = false;
	}
	
	public Action<Exception> ErrorHandler { get; }

	protected class TimerData
	{
		
		public static TimerData GetFromPool()
		{
			if (_head is null)
			{
				return new();
			}
			else
			{
				TimerData data = _head;
				_head = data._next;
				return data;
			}
		}

		public void ReturnToPool()
		{
			if (_head is null)
			{
				_head = this;
			}
			else
			{
				_next = _head;
				_head = this;
			}
		}
		
		// The following properties need to initialize manually.
		public TimeSpan StartTime { get; set; }
		public Action<TimeSpan, object?> Callback { get; set; } = null!;
		public object? State { get; set; }
		public TimeSpan Rate { get; set; }
		public bool IsLooped { get; set; }
		public Lifecycle Lifecycle { get; set; }
		public Action<LifecycleExpiredException, object?>? OnExpired { get; set; }
		public uint64 SuspendVersion { get; set; }
		
		public TimeSpan TriggerTime => StartTime + Rate;

		private static TimerData? _head;

		private TimerData? _next;
		
	}

	protected abstract void Advance(TimerData data);
	protected abstract void BeginTick();
	protected abstract void EndTick();

	protected abstract bool IsOverBudget { get; }

	protected TimeSpan AccumulatedTime => _accumulatedSeconds.TimeSpan;

	private static void DefaultErrorHandler(Exception exception) => UE_ERROR(LogTemp, $"Unhandled Exception Detected in Timer Scheduler.\n{exception}");

	private bool InternalIsValidTimer(Timer timer) => timer.Owner == this && (_registry.ContainsKey(timer) || _suspendedRegistry.ContainsKey(timer));

	private void TakeSnapshot(Timer timer, TimerData data) => _snapshots.Enqueue((timer, data.SuspendVersion), data.TriggerTime);
	
	private static readonly TimeSpan _minLoopRate = TimeSpan.FromMilliseconds(1);

	private Dictionary<Timer, TimerData> _registry = new();
	private Dictionary<Timer, TimerData> _suspendedRegistry = new();
	private PriorityQueue<(Timer, uint64), TimeSpan> _snapshots = new();

	private uint64 _handle = 0;

	private AccumulatedSeconds _accumulatedSeconds;

	private bool _ticking;

}


