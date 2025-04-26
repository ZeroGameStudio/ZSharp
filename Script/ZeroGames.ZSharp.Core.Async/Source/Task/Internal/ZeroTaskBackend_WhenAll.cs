// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

internal class ZeroTaskBackend_WhenAll<TResult> : ZeroTaskBackendBase<TResult[]>
{

	public ZeroTaskBackend_WhenAll(IEnumerable<ZeroTask<TResult>> tasks)
	{
		ZeroTask<TResult>[] taskArr = tasks.ToArray();
		
		_count = taskArr.Length;
		_results = _count > 0 && typeof(TResult) != typeof(AsyncVoid) ? new TResult[_count] : [];
		
		for (int32 i = 0; i < _count; ++i)
		{
			ZeroTask<TResult> task = taskArr[i];
			ZeroTask<TResult>.Awaiter awaiter = task.GetAwaiter();
			if (awaiter.IsCompleted)
			{
				MoveNext(awaiter, i);
			}
			else
			{
				int32 upperIndex = i;
				awaiter.OnCompleted(() => MoveNext(awaiter, upperIndex));
			}
		}
		
		CheckCompletion();
	}

	private void MoveNext(ZeroTask<TResult>.Awaiter awaiter, int32 index)
	{
		++_completedCount;
		try
		{
			TResult result = awaiter.GetResult();
			if (typeof(TResult) != typeof(AsyncVoid))
			{
				_results[index] = result;
			}
			CheckCompletion();
		}
		catch (Exception ex)
		{
			// We can only catch the first exception, the others will be forwarded to unobserved publisher.
			if (GetStatus(Token) is EZeroTaskStatus.Pending)
			{
				SetException(ex);
			}
			else
			{
				UnobservedZeroTaskExceptionPublisher.Publish(ex);
			}
		}
	}

	private void CheckCompletion()
	{
		if (GetStatus(Token) is EZeroTaskStatus.Pending && _completedCount == _count)
		{
			SetResult(_results);
		}
	}

	private readonly TResult[] _results;
	private readonly int32 _count;
	private int32 _completedCount;

}


