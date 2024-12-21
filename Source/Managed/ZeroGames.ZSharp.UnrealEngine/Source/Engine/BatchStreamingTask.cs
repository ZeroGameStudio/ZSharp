// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public readonly record struct BatchStreamingProgress(int32 LoadedCount, int32 TotalCount);

public class BatchStreamingTask<T> : StreamingTaskBase, IAwaitable<T[], BatchStreamingTask<T>.Awaiter> where T : UnrealObject
{

	public readonly struct Awaiter : IAwaiter<T[]>, IMoveNextSourceAwaiter
	{
		void IMoveNextSourceAwaiter.OnCompleted(IMoveNextSource source) => _task.ContinueWith(source);
		public void OnCompleted(Action continuation) => _task.ContinueWith(continuation);

		public T[] GetResult() => _task.Result;
		void IAwaiter.GetResult() => GetResult();
		
		public bool IsCompleted => _task.IsCompleted;
		
		internal Awaiter(BatchStreamingTask<T> task) => _task = task;
		
		private readonly BatchStreamingTask<T> _task;
	}

	public Awaiter GetAwaiter() => new(this);

	public T[] Result
	{
		get
		{
			GuardInvariant();
			
			if (!IsCompleted)
			{
				throw new InvalidOperationException("Operation incomplete.");
			}
			
			TryGetException();

			return InternalResult;
		}
	}

	public int32 LoadedCount
	{
		get
		{
			GuardInvariant();

			if (_progress is null)
			{
				FetchLoadedCount();
			}

			return _loadedCount;
		}
	}

	internal BatchStreamingTask(IntPtr unmanaged, int32 totalCount, Lifecycle lifecycle, IProgress<BatchStreamingProgress>? progress) : base(unmanaged, lifecycle)
	{
		_totalCount = totalCount;
		_progress = progress;
	}
	
	internal static BatchStreamingTask<T> EmptyTask => throw new NotImplementedException();

	protected override void InternalUpdate(int32 loadedCount)
	{
		_loadedCount = loadedCount;
		_progress?.Report(new(_loadedCount, _totalCount));
	}

	private unsafe void FetchLoadedCount() => _loadedCount = StreamingTask_Interop.GetLoadedCount(Unmanaged);

	private unsafe T[] InternalResult
	{
		get
		{
			if (!_cached)
			{
				const int32 STACK_ALLOC_THRESHOLD = 2 << 4;
				
				_cached = true;

				if (_totalCount <= STACK_ALLOC_THRESHOLD)
				{
					ConjugateHandle* buffer = stackalloc ConjugateHandle[STACK_ALLOC_THRESHOLD];
					int32 writeCount = StreamingTask_Interop.GetResult(Unmanaged, buffer, _totalCount);
					ensure(writeCount == _totalCount);
					List<T> list = new(writeCount);
					for (int32 i = 0; i < writeCount; ++i)
					{
						if (buffer[i].GetTarget<T>() is { } target)
						{
							list.Add(target);
						}
					}

					_result = list.ToArray();
				}
				else
				{
					ConjugateHandle[] buffer = new ConjugateHandle[_totalCount];
					int32 writeCount;
					fixed (ConjugateHandle* fixedBuffer = buffer)
					{
						writeCount = StreamingTask_Interop.GetResult(Unmanaged, fixedBuffer, _totalCount);
					}
					ensure(writeCount == _totalCount);
					List<T> list = new(writeCount);
					for (int32 i = 0; i < writeCount; ++i)
					{
						if (buffer[i].GetTarget<T>() is { } target)
						{
							list.Add(target);
						}
					}

					_result = list.ToArray();
				}
			}
			
			return _result ?? [];
		}
	}

	private int32 _loadedCount;
	private readonly int32 _totalCount;

	private IProgress<BatchStreamingProgress>? _progress;

	private bool _cached;
	private T[]? _result;
	
}


