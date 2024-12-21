﻿// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public class StreamingTask<T> : StreamingTaskBase, IAwaitable<T?, StreamingTask<T>.Awaiter> where T : UnrealObject
{

	public readonly struct Awaiter : IAwaiter<T?>, IMoveNextSourceAwaiter
	{
		void IMoveNextSourceAwaiter.OnCompleted(IMoveNextSource source) => _task.ContinueWith(source);
		public void OnCompleted(Action continuation) => _task.ContinueWith(continuation);

		public T? GetResult() => _task.Result;
		void IAwaiter.GetResult() => GetResult();
		
		public bool IsCompleted => _task.IsCompleted;
		
		internal Awaiter(StreamingTask<T> task) => _task = task;
		
		private readonly StreamingTask<T> _task;
	}

	public Awaiter GetAwaiter() => new(this);

	public T? Result
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
	
	internal StreamingTask(IntPtr unmanaged, Lifecycle lifecycle) : base(unmanaged, lifecycle){}

	internal static StreamingTask<T> EmptyTask => throw new NotImplementedException();

	protected override void InternalUpdate(int32 loadedCount) => throw new NotSupportedException();

	private unsafe T? InternalResult
	{
		get
		{
			if (!_cached)
			{
				_cached = true;
				ConjugateHandle handle = default;
				ensure(StreamingTask_Interop.GetResult(Unmanaged, &handle, 1) == 1);
				_result = handle.GetTarget<T>();
			}
			
			return _result;
		}
	}

	private bool _cached;
	private T? _result;
	
}


