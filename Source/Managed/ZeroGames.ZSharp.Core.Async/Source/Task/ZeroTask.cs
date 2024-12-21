// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;

namespace ZeroGames.ZSharp.Core.Async;

[AsyncMethodBuilder(typeof(AsyncZeroTaskMethodBuilderVoid))]
public readonly partial struct ZeroTask : IAwaitable<ZeroTask.Awaiter>, IEquatable<ZeroTask>
{
	
	public readonly struct Awaiter : IAwaiter, IMoveNextSourceAwaiter
	{
		
		void IMoveNextSourceAwaiter.OnCompleted(IMoveNextSource source)
		{
			Thrower.ThrowIfNotInGameThread();
			_task.SetMoveNextSource(source);
		}

		public void OnCompleted(Action continuation)
		{
			Thrower.ThrowIfNotInGameThread();
			_task.SetContinuation(continuation);
		}

		public void GetResult()
		{
			Thrower.ThrowIfNotInGameThread();
			_task.GetResult();
		}

		public bool IsCompleted
		{
			get
			{
				Thrower.ThrowIfNotInGameThread();
				return _task.IsCompleted;
			}
		}
		
		internal Awaiter(ZeroTask task) => _task = task;

		private readonly ZeroTask _task;

	}

	public Awaiter GetAwaiter() => new(this);

	public bool Equals(ZeroTask other) => _underlyingTask == other._underlyingTask && _tokenSnapshot == other._tokenSnapshot;
	public override bool Equals(object? obj) => obj is ZeroTask other && Equals(other);
	public override int32 GetHashCode() => _underlyingTask?.GetHashCode() ?? 0;
	public static bool operator ==(ZeroTask lhs, ZeroTask rhs) => lhs.Equals(rhs);
	public static bool operator !=(ZeroTask lhs, ZeroTask rhs) => !lhs.Equals(rhs);
	
	internal ZeroTask(IUnderlyingZeroTask underlyingTask)
	{
		_underlyingTask = underlyingTask;
		_tokenSnapshot = underlyingTask.Token;
	}

	private bool IsCompleted => _underlyingTask is null || _underlyingTask.GetStatus(_tokenSnapshot) != EUnderlyingZeroTaskStatus.Pending;

	private void GetResult()
	{
		if (_underlyingTask is not null)
		{
			_underlyingTask.GetResult(_tokenSnapshot);
		}
	}
	
	private void SetMoveNextSource(IMoveNextSource source)
	{
		if (_underlyingTask is null)
		{
			source.MoveNext();
		}
		else
		{
			_underlyingTask.SetMoveNextSource(source, _tokenSnapshot);
		}
	}

	private void SetContinuation(Action continuation)
	{
		if (_underlyingTask is null)
		{
			continuation();
		}
		else
		{
			_underlyingTask.SetContinuation(continuation, _tokenSnapshot);
		}
	}
	
	private readonly IUnderlyingZeroTask? _underlyingTask;
	private readonly UnderlyingZeroTaskToken _tokenSnapshot;

}

[AsyncMethodBuilder(typeof(AsyncZeroTaskMethodBuilder<>))]
public readonly struct ZeroTask<TResult> : IAwaitable<TResult, ZeroTask<TResult>.Awaiter>, IEquatable<ZeroTask<TResult>>
{
	
	public readonly struct Awaiter : IAwaiter<TResult>, IMoveNextSourceAwaiter
	{
		
		void IMoveNextSourceAwaiter.OnCompleted(IMoveNextSource source)
		{
			Thrower.ThrowIfNotInGameThread();
			_task.SetMoveNextSource(source);
		}

		public void OnCompleted(Action continuation)
		{
			Thrower.ThrowIfNotInGameThread();
			_task.SetContinuation(continuation);
		}

		void IAwaiter.GetResult() => GetResult();
		public TResult GetResult()
		{
			Thrower.ThrowIfNotInGameThread();
			return _task.GetResult();
		}

		public bool IsCompleted
		{
			get
			{
				Thrower.ThrowIfNotInGameThread();
				return _task.IsCompleted;
			}
		}
		
		
		internal Awaiter(ZeroTask<TResult> task) => _task = task;

		private readonly ZeroTask<TResult> _task;

	}

	public Awaiter GetAwaiter() => new(this);

	public bool Equals(ZeroTask<TResult> other) => _underlyingTask == other._underlyingTask && _tokenSnapshot == other._tokenSnapshot;
	public override bool Equals(object? obj) => obj is ZeroTask<TResult> other && Equals(other);
	public override int32 GetHashCode() => _underlyingTask?.GetHashCode() ?? 0;
	public static bool operator ==(ZeroTask<TResult> lhs, ZeroTask<TResult> rhs) => lhs.Equals(rhs);
	public static bool operator !=(ZeroTask<TResult> lhs, ZeroTask<TResult> rhs) => !lhs.Equals(rhs);

	public static implicit operator ZeroTask(ZeroTask<TResult> @this)
	{
		if (@this._underlyingTask is null)
		{
			return ZeroTask.CompletedTask;
		}

		return new(@this._underlyingTask!);
	}
	
	internal ZeroTask(TResult inlineResult)
	{
		_inlineResult = inlineResult;
	}

	internal ZeroTask(IUnderlyingZeroTask<TResult> underlyingTask)
	{
		_underlyingTask = underlyingTask;
		_tokenSnapshot = underlyingTask.Token;
	}

	private bool IsCompleted => _underlyingTask is null || _underlyingTask.GetStatus(_tokenSnapshot) != EUnderlyingZeroTaskStatus.Pending;

	private TResult GetResult()
	{
		if (_underlyingTask is not null)
		{
			return _underlyingTask.GetResult(_tokenSnapshot);
		}

		return _inlineResult!;
	}
	
	private void SetMoveNextSource(IMoveNextSource source)
	{
		if (_underlyingTask is null)
		{
			source.MoveNext();
		}
		else
		{
			_underlyingTask.SetMoveNextSource(source, _tokenSnapshot);
		}
	}

	private void SetContinuation(Action continuation)
	{
		if (_underlyingTask is null)
		{
			continuation();
		}
		else
		{
			_underlyingTask.SetContinuation(continuation, _tokenSnapshot);
		}
	}

	private readonly TResult? _inlineResult;
	private readonly IUnderlyingZeroTask<TResult>? _underlyingTask;
	private readonly UnderlyingZeroTaskToken _tokenSnapshot;
	
}


