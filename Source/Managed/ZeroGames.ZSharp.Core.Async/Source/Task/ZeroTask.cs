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

	public bool Equals(ZeroTask other) => _backend == other._backend && _tokenSnapshot == other._tokenSnapshot;
	public override bool Equals(object? obj) => obj is ZeroTask other && Equals(other);
	public override int32 GetHashCode() => _backend?.GetHashCode() ?? 0;
	public static bool operator ==(ZeroTask lhs, ZeroTask rhs) => lhs.Equals(rhs);
	public static bool operator !=(ZeroTask lhs, ZeroTask rhs) => !lhs.Equals(rhs);
	
	internal ZeroTask(IZeroTaskBackend backend)
	{
		_backend = backend;
		_tokenSnapshot = backend.Token;
	}

	private bool IsCompleted => _backend is null || _backend.GetStatus(_tokenSnapshot) != EZeroTaskStatus.Pending;

	private void GetResult()
	{
		if (_backend is not null)
		{
			_backend.GetResult(_tokenSnapshot);
		}
	}
	
	private void SetMoveNextSource(IMoveNextSource source)
	{
		if (_backend is null)
		{
			source.MoveNext();
		}
		else
		{
			_backend.SetMoveNextSource(source, _tokenSnapshot);
		}
	}

	private void SetContinuation(Action continuation)
	{
		if (_backend is null)
		{
			continuation();
		}
		else
		{
			_backend.SetContinuation(continuation, _tokenSnapshot);
		}
	}
	
	private readonly IZeroTaskBackend? _backend;
	private readonly ZeroTaskToken _tokenSnapshot;

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

	public bool Equals(ZeroTask<TResult> other) => _backend == other._backend && _tokenSnapshot == other._tokenSnapshot;
	public override bool Equals(object? obj) => obj is ZeroTask<TResult> other && Equals(other);
	public override int32 GetHashCode() => _backend?.GetHashCode() ?? 0;
	public static bool operator ==(ZeroTask<TResult> lhs, ZeroTask<TResult> rhs) => lhs.Equals(rhs);
	public static bool operator !=(ZeroTask<TResult> lhs, ZeroTask<TResult> rhs) => !lhs.Equals(rhs);

	public static implicit operator ZeroTask(ZeroTask<TResult> @this)
	{
		if (@this._backend is null)
		{
			return ZeroTask.CompletedTask;
		}

		return new(@this._backend!);
	}
	
	internal ZeroTask(TResult inlineResult)
	{
		_inlineResult = inlineResult;
	}

	internal ZeroTask(IZeroTaskBackend<TResult> backend)
	{
		_backend = backend;
		_tokenSnapshot = backend.Token;
	}

	private bool IsCompleted => _backend is null || _backend.GetStatus(_tokenSnapshot) != EZeroTaskStatus.Pending;

	private TResult GetResult()
	{
		if (_backend is not null)
		{
			return _backend.GetResult(_tokenSnapshot);
		}

		return _inlineResult!;
	}
	
	private void SetMoveNextSource(IMoveNextSource source)
	{
		if (_backend is null)
		{
			source.MoveNext();
		}
		else
		{
			_backend.SetMoveNextSource(source, _tokenSnapshot);
		}
	}

	private void SetContinuation(Action continuation)
	{
		if (_backend is null)
		{
			continuation();
		}
		else
		{
			_backend.SetContinuation(continuation, _tokenSnapshot);
		}
	}

	private readonly TResult? _inlineResult;
	private readonly IZeroTaskBackend<TResult>? _backend;
	private readonly ZeroTaskToken _tokenSnapshot;
	
}


