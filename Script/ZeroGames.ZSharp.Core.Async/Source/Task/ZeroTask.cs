// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;

namespace ZeroGames.ZSharp.Core.Async;

[AsyncMethodBuilder(typeof(AsyncZeroTaskMethodBuilderVoid))]
public readonly partial struct ZeroTask : IAwaitable<ZeroTask.Awaiter>
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

	internal ZeroTask(IZeroTaskBackend backend)
	{
		_backend = backend;
		_token = backend.Token;
	}

	private bool IsCompleted => _backend is null || _backend.GetStatus(_token) != EZeroTaskStatus.Pending;

	private void GetResult()
	{
		if (_backend is not null)
		{
			_backend.GetResult(_token);
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
			_backend.SetMoveNextSource(source, _token);
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
			_backend.SetContinuation(continuation, _token);
		}
	}
	
	private readonly IZeroTaskBackend? _backend;
	private readonly ZeroTaskToken _token;

}

[AsyncMethodBuilder(typeof(AsyncZeroTaskMethodBuilder<>))]
public readonly partial struct ZeroTask<TResult> : IAwaitable<TResult, ZeroTask<TResult>.Awaiter>
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

	internal ZeroTask(TResult result)
	{
		_result = result;
	}

	internal ZeroTask(IZeroTaskBackend<TResult> backend)
	{
		_backend = backend;
		_token = backend.Token;
	}

	private bool IsCompleted => _backend is null || _backend.GetStatus(_token) != EZeroTaskStatus.Pending;

	private TResult GetResult()
	{
		if (_backend is not null)
		{
			return _backend.GetResult(_token);
		}

		return _result!;
	}
	
	private void SetMoveNextSource(IMoveNextSource source)
	{
		if (_backend is null)
		{
			source.MoveNext();
		}
		else
		{
			_backend.SetMoveNextSource(source, _token);
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
			_backend.SetContinuation(continuation, _token);
		}
	}

	private readonly TResult? _result;
	private readonly IZeroTaskBackend<TResult>? _backend;
	private readonly ZeroTaskToken _token;
	
}


