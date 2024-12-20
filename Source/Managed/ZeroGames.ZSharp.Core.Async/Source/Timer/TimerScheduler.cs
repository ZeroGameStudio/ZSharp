// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;
using System.Numerics;

namespace ZeroGames.ZSharp.Core.Async;

public sealed class TimerScheduler<T> : ITimerScheduler<T> where T : struct, INumber<T>
{

	public TimerScheduler(T minRate) : this(minRate, true, null){}
	public TimerScheduler(T minRate, bool reliable) : this(minRate, reliable, null){}
	public TimerScheduler(T minRate, Action<Exception>? errorHandler) : this(minRate, true, errorHandler){}
	public TimerScheduler(T minRate, bool reliable, Action<Exception>? errorHandler)
	{
		ArgumentOutOfRangeException.ThrowIfNegative(minRate, nameof(minRate));
		
		_minRate = minRate;
		_stopwatch = reliable ? null : new();
		_timeBudget = reliable ? TimeSpan.Zero : TimeSpan.FromMilliseconds(1);
		_timeLimit = reliable ? TimeSpan.Zero : TimeSpan.FromMilliseconds(2);
		ErrorHandler = errorHandler ?? DefaultErrorHandler;
	}
	
	public Timer Register(Action<T> callback, T rate, bool looped = false, bool fixedRate = true, Lifecycle lifecycle = default, Action<LifecycleExpiredException, object?>? onExpired = null)
	{
		Thrower.ThrowIfNotInGameThread();
		if (looped && rate < _minRate)
		{
			rate = _minRate;
		}

		return InternalRegister(callback, null, null, rate, looped, fixedRate, lifecycle, onExpired);
	}

	public Timer Register(Action<T, object?> callback, object? state, T rate, bool looped = false, bool fixedRate = true, Lifecycle lifecycle = default, Action<LifecycleExpiredException, object?>? onExpired = null)
	{
		Thrower.ThrowIfNotInGameThread();
		if (looped && rate < _minRate)
		{
			rate = _minRate;
		}

		return InternalRegister(null, callback, state, rate, looped, fixedRate, lifecycle, onExpired);
	}

	public void Unregister(Timer timer)
	{
		Thrower.ThrowIfNotInGameThread();
		if (timer.Owner != this)
		{
			return;
		}
		
		InternalUnregister(timer);
	}

	public void UnregisterAll(Lifecycle lifecycle)
	{
		Thrower.ThrowIfNotInGameThread();
		throw new NotImplementedException();
	}
	
	public void Suspend(Timer timer)
	{
		Thrower.ThrowIfNotInGameThread();
		if (timer.Owner != this)
		{
			return;
		}
		
		InternalSuspend(timer);
	}

	public void SuspendAll(Lifecycle lifecycle)
	{
		Thrower.ThrowIfNotInGameThread();
		throw new NotImplementedException();
	}

	public void Resume(Timer timer)
	{
		Thrower.ThrowIfNotInGameThread();
		if (timer.Owner != this)
		{
			return;
		}
		
		InternalResume(timer);
	}

	public void ResumeAll(Lifecycle lifecycle)
	{
		Thrower.ThrowIfNotInGameThread();
		throw new NotImplementedException();
	}

	public bool IsValidTimer(Timer timer)
	{
		Thrower.ThrowIfNotInGameThread();
		return InternalIsValidTimer(timer);
	}

	public void Tick(T deltaTime)
	{
		Thrower.ThrowIfNotInGameThread();
		ArgumentOutOfRangeException.ThrowIfLessThanOrEqual(deltaTime, T.Zero, nameof(deltaTime));

		if (!ensure(!_ticking))
		{
			return;
		}
		
		_ticking = true;
		
		_roundedAccumulatedTime.Advance(deltaTime, out _accumulatedTime);

		BeginTick();

		while (_snapshots.TryPeek(out var snapshot, out var triggerTimeSnapshop))
		{
			T accumulatedTime = _accumulatedTime;
			if (triggerTimeSnapshop > accumulatedTime)
			{
				// 1. This is a min heap so there is no need to continue to iterate anymore.
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
				// IMPORTANT: Advance must be done before user code executes so that Suspend/Resume works fine inside user code.
				GuardedInvoke(data, Advance(data), ref expired);
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
			
			// If we are out of limit, defer remaining task to next tick.
			if (IsOverLimit)
			{
				break;
			}
		}
		
		EndTick();

		_ticking = false;
	}

	public Action<Exception> ErrorHandler { get; }
	
	public bool IsReliable => _stopwatch is null;

	public TimeSpan TimeBudget
	{
		get
		{
			if (IsReliable)
			{
				throw new InvalidOperationException();
			}

			return _timeBudget;
		}
		set
		{
			if (IsReliable)
			{
				throw new InvalidOperationException();
			}
			
			_timeBudget = value;
		}
	}
	
	public TimeSpan TimeLimit
	{
		get
		{
			if (IsReliable)
			{
				throw new InvalidOperationException();
			}

			return _timeLimit;
		}
		set
		{
			if (IsReliable)
			{
				throw new InvalidOperationException();
			}
			
			_timeLimit = value;
		}
	}

	private class TimerData
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
		public T StartTime { get; set; }
		public Action<T>? StatelessCallback { get; set; }
		public Action<T, object?>? StatefulCallback { get; set; }
		public object? State { get; set; }
		public T Rate { get; set; }
		public bool IsLooped { get; set; }
		public bool IsFixedRate { get; set; }
		public Lifecycle Lifecycle { get; set; }
		public Action<LifecycleExpiredException, object?>? OnExpired { get; set; }
		public uint64 SuspendVersion { get; set; }
		
		public T TriggerTime => StartTime + Rate;

		private static TimerData? _head;

		private TimerData? _next;
		
	}

	private struct RoundedAccumulatedTime
	{
		public void Advance(T deltaTime, out T accumulatedTime)
		{
			// It seems like we can't know whether T is floating point type at compile time
			// because IFloatingPointIeee754<TSelf> uses CRTP...
			if (_floatingPointNumber)
			{
				_accumulatedTime += deltaTime;
				while (_accumulatedTime > _timePerRound)
				{
					_round += T.One;
					_accumulatedTime -= _timePerRound;
				}

				accumulatedTime = _timePerRound * _round + _accumulatedTime;
			}
			else
			{
				_accumulatedTime += deltaTime;
				accumulatedTime = _accumulatedTime;
			}
		}

		private static readonly bool _floatingPointNumber = typeof(T).GetInterfaces().Any(i => i.IsGenericType && i.GetGenericTypeDefinition() == typeof(IFloatingPointIeee754<>));
		private static readonly T _timePerRound = T.CreateTruncating(2 << 10);
		private T _round;
		private T _accumulatedTime;
	}

	private void BeginTick() => _stopwatch?.Restart();
	private void EndTick() => _stopwatch?.Stop();

	private static void DefaultErrorHandler(Exception exception) => UnhandledExceptionHelper.Guard(exception, "Unhandled Exception Detected in Timer Scheduler.");

	private Timer InternalRegister(Action<T>? statelessCallback, Action<T, object?>? statefulCallback, object? state, T rate, bool looped = false, bool fixedRate = true, Lifecycle lifecycle = default, Action<LifecycleExpiredException, object?>? onExpired = null)
	{
		Timer timer = new(this, ++_handle);
		TimerData data = TimerData.GetFromPool();
		data.StartTime = _accumulatedTime;
		data.StatelessCallback = statelessCallback;
		data.StatefulCallback = statefulCallback;
		data.State = state;
		data.Rate = rate;
		data.IsLooped = looped;
		data.IsFixedRate = fixedRate;
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
	
	private bool InternalIsValidTimer(Timer timer) => timer.Owner == this && (_registry.ContainsKey(timer) || _suspendedRegistry.ContainsKey(timer));

	private void TakeSnapshot(Timer timer, TimerData data) => _snapshots.Enqueue((timer, data.SuspendVersion), data.TriggerTime);

	private T Advance(TimerData data)
	{
		if (data.IsFixedRate && !IsOverBudget)
		{
			data.StartTime += data.Rate;
			return data.Rate;
		}
		else
		{
			data.StartTime = _accumulatedTime;
			return _accumulatedTime - data.StartTime;
		}
	}

	private void GuardedInvoke(TimerData data, T deltaTime, ref bool expired)
	{
		try
		{
			if (!data.Lifecycle.IsExpired)
			{
				if (data.StatelessCallback is not null)
				{
					data.StatelessCallback(deltaTime);
				}
				else
				{
					data.StatefulCallback!(deltaTime, data.State);
				}
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
	
	private bool IsOverBudget => _stopwatch is not null && TimeBudget > TimeSpan.Zero && TimeSpan.FromTicks(_stopwatch.ElapsedTicks) > TimeBudget;
	private bool IsOverLimit => _stopwatch is not null && TimeLimit > TimeSpan.Zero && TimeSpan.FromTicks(_stopwatch.ElapsedTicks) > TimeLimit;
	
	private readonly Dictionary<Timer, TimerData> _registry = new();
	private readonly Dictionary<Timer, TimerData> _suspendedRegistry = new();
	private readonly PriorityQueue<(Timer, uint64), T> _snapshots = new();

	private uint64 _handle;

	private bool _ticking;
	
	private readonly T _minRate;
	
	private T _accumulatedTime;
	private RoundedAccumulatedTime _roundedAccumulatedTime;

	private readonly Stopwatch? _stopwatch;
	private TimeSpan _timeBudget;
	private TimeSpan _timeLimit;

}


