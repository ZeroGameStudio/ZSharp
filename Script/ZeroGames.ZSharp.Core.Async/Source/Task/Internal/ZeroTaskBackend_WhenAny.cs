// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

internal class ZeroTaskBackend_WhenAny<TResult> : ZeroTaskBackendBase<(int32 WinnerIndex, TResult Result)>
{

	public ZeroTaskBackend_WhenAny(IEnumerable<ZeroTask<TResult>> tasks)
	{
		int32 count = 0;
		foreach (var task in tasks)
		{
			int32 index = count;
			++count;
			ZeroTask<TResult>.Awaiter awaiter = task.GetAwaiter();
			if (awaiter.IsCompleted)
			{
				MoveNext(awaiter, index);
			}
			else
			{
				awaiter.OnCompleted(() => MoveNext(awaiter, index));
			}
		}

		if (count is 0)
		{
			SetException(new LifetimeExpiredException(Lifetime.Expired));
		}
	}

	private void MoveNext(ZeroTask<TResult>.Awaiter awaiter, int32 index)
	{
		try
		{
			TResult result = awaiter.GetResult();
			if (GetStatus(Token) is EZeroTaskStatus.Pending)
			{
				SetResult((index, result));
			}
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

}


