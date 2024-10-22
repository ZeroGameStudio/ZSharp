// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;

namespace ZeroGames.ZSharp.Core.Async;

[AsyncMethodBuilder(typeof(AsyncZeroTaskMethodBuilderVoid))]
public readonly partial struct ZeroTask : IZeroTask, IAwaitable<ZeroTask.Awaiter>, IEquatable<ZeroTask>
{
	
	public readonly struct Awaiter(ZeroTask task) : IAwaiter, IZeroTaskAwaiter
	{
		
		public void OnCompleted(IAsyncStateMachine stateMachine)
		{
			ThreadHelper.ValidateGameThread();
			_task.SetStateMachine(stateMachine);
		}

		public void OnCompleted(Action continuation)
		{
			ThreadHelper.ValidateGameThread();
			_task.SetContinuation(continuation);
		}

		public void GetResult()
		{
			ThreadHelper.ValidateGameThread();
			_task.GetResult();
		}

		public bool IsCompleted
		{
			get
			{
				ThreadHelper.ValidateGameThread();
				return _task.IsCompleted;
			}
		}

		private readonly ZeroTask _task = task;

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
	
	private void SetStateMachine(IAsyncStateMachine stateMachine)
	{
		if (_underlyingTask is null)
		{
			stateMachine.MoveNext();
		}
		else
		{
			_underlyingTask.SetStateMachine(stateMachine, _tokenSnapshot);
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

public readonly partial struct ZeroTask<TResult> : IZeroTask<TResult>, IAwaitable<TResult, ZeroTask<TResult>.Awaiter>, IEquatable<ZeroTask<TResult>>
{
	
	public readonly struct Awaiter(ZeroTask<TResult> task) : IAwaiter<TResult>, IZeroTaskAwaiter
	{
		
		public void OnCompleted(IAsyncStateMachine stateMachine)
		{
			ThreadHelper.ValidateGameThread();
			_task.SetStateMachine(stateMachine);
		}

		public void OnCompleted(Action continuation)
		{
			ThreadHelper.ValidateGameThread();
			_task.SetContinuation(continuation);
		}

		void IAwaiter.GetResult() => GetResult();
		public TResult GetResult()
		{
			ThreadHelper.ValidateGameThread();
			return _task.GetResult();
		}

		public bool IsCompleted
		{
			get
			{
				ThreadHelper.ValidateGameThread();
				return _task.IsCompleted;
			}
		}

		private readonly ZeroTask<TResult> _task = task;

	}

	public Awaiter GetAwaiter() => new(this);

	public bool Equals(ZeroTask<TResult> other) => _underlyingTask == other._underlyingTask && _tokenSnapshot == other._tokenSnapshot;
	public override bool Equals(object? obj) => obj is ZeroTask<TResult> other && Equals(other);
	public override int32 GetHashCode() => _underlyingTask?.GetHashCode() ?? 0;
	public static bool operator ==(ZeroTask<TResult> lhs, ZeroTask<TResult> rhs) => lhs.Equals(rhs);
	public static bool operator !=(ZeroTask<TResult> lhs, ZeroTask<TResult> rhs) => !lhs.Equals(rhs);

	public static implicit operator ZeroTask(ZeroTask<TResult> @this) => @this.ToZeroTask();

	public ZeroTask ToZeroTask()
	{
		if (_underlyingTask is null)
		{
			return ZeroTask.CompletedTask;
		}

		return new(_underlyingTask!);
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
	
	private void SetStateMachine(IAsyncStateMachine stateMachine)
	{
		if (_underlyingTask is null)
		{
			stateMachine.MoveNext();
		}
		else
		{
			_underlyingTask.SetStateMachine(stateMachine, _tokenSnapshot);
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


