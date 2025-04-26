// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

internal sealed class ZeroStream_WhenEach<TResult> : IZeroStream<(TResult? Result, Exception? Exception)>
{

	public ZeroStream_WhenEach(IEnumerable<ZeroTask<TResult>> tasks)
	{
		ZeroTask<TResult>[] taskArr = tasks.ToArray();
		_desiredCount = taskArr.Length;
		_storage = new(_desiredCount);
		foreach (var task in taskArr)
		{
			ZeroTask<TResult>.Awaiter awaiter = task.GetAwaiter();
			if (awaiter.IsCompleted)
			{
				AddResult(awaiter);
			}
			else
			{
				awaiter.OnCompleted(() => AddResult(awaiter));
			}
		}
	}

	public IZeroStreamEnumerator<(TResult? Result, Exception? Exception)> GetAsyncEnumerator()
	{
		Thrower.ThrowIfNotInGameThread();
		return new Enumerator(this);
	}

	private class Enumerator : IZeroStreamEnumerator<(TResult? Result, Exception? Exception)>
	{
		public ZeroTask DisposeAsync()
		{
			_disposed = true;
			return ZeroTask.CompletedTask;
		}

		public async ZeroTask<bool> MoveNextAsync()
		{
			if (_disposed)
			{
				throw new InvalidOperationException();
			}
			
			while (++_index < _target._storage.Count)
			{
				_current = _target._storage[_index];
				return true;
			}
			
			if (_index == _target._desiredCount)
			{
				return false;
			}
			
			ZeroTaskCompletionSource tcs = ZeroTaskCompletionSource.Create();
			_target._tcs ??= [];
			_target._tcs.Enqueue(tcs);
			await tcs.Task;
			
			_current = _target._storage[_index];
			return true;
		}

		public (TResult? Result, Exception? Exception) Current
		{
			get
			{
				if (_disposed)
				{
					throw new InvalidOperationException();
				}

				if (_current is null)
				{
					throw new InvalidOperationException();
				}

				return _current.Value;
			}
		}
		
		internal Enumerator(ZeroStream_WhenEach<TResult> target)
		{
			_target = target;
		}

		private readonly ZeroStream_WhenEach<TResult> _target;
		private int32 _index = -1;
		private (TResult? Result, Exception? Exception)? _current;
		private bool _disposed;
	}

	private void AddResult(ZeroTask<TResult>.Awaiter awaiter)
	{
		try
		{
			_storage.Add((awaiter.GetResult(), null));
		}
		catch (Exception ex)
		{
			_storage.Add((default, ex));
		}
		
		if (_tcs is not null)
		{
			// tcs.SetResult() may change the queue so we swap it.
			Queue<ZeroTaskCompletionSource> tempTcs = _tcs;
			_tcs = null;
			while (tempTcs.TryDequeue(out var tcs))
			{
				tcs.SetResult();
			}
		}
	}

	private readonly List<(TResult? Result, Exception? Exception)> _storage;
	private readonly int32 _desiredCount;
	private Queue<ZeroTaskCompletionSource>? _tcs;

}