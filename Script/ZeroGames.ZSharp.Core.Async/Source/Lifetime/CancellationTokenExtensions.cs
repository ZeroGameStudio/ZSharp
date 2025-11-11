// Copyright Zero Games. All Rights Reserved.

using System.Threading;

namespace ZeroGames.ZSharp.Core.Async;

public static class CancellationTokenExtensions
{
	extension(CancellationToken @this)
	{
		public CancellationTokenRegistration RegisterWithoutCaptureExecutionContext(Action callback)
		{
			var restoreFlow = false;
			if (!ExecutionContext.IsFlowSuppressed())
			{
				ExecutionContext.SuppressFlow();
				restoreFlow = true;
			}

			try
			{
				return @this.Register(() => IGameThreadScheduler.Instance.Send(static state => ((Action)state!)(), callback), false);
			}
			finally
			{
				if (restoreFlow)
				{
					ExecutionContext.RestoreFlow();
				}
			}
		}

		public CancellationTokenRegistration RegisterWithoutCaptureExecutionContext(Action<object?> callback, object? state)
		{
			var restoreFlow = false;
			if (!ExecutionContext.IsFlowSuppressed())
			{
				ExecutionContext.SuppressFlow();
				restoreFlow = true;
			}

			try
			{
				return @this.Register(_ => IGameThreadScheduler.Instance.Send(_ => callback(state), null), null, false);
			}
			finally
			{
				if (restoreFlow)
				{
					ExecutionContext.RestoreFlow();
				}
			}
		}
	}
}


