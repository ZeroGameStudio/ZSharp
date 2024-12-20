// Copyright Zero Games. All Rights Reserved.

using System.Threading;

namespace ZeroGames.ZSharp.Core.Async;

public static class CancellationTokenExtensions
{
	
	public static CancellationTokenRegistration RegisterWithoutCaptureExecutionContext(this CancellationToken @this, Action callback)
	{
		var restoreFlow = false;
		if (!ExecutionContext.IsFlowSuppressed())
		{
			ExecutionContext.SuppressFlow();
			restoreFlow = true;
		}

		try
		{
			return @this.Register(callback, false);
		}
		finally
		{
			if (restoreFlow)
			{
				ExecutionContext.RestoreFlow();
			}
		}
	}

	public static CancellationTokenRegistration RegisterWithoutCaptureExecutionContext(this CancellationToken @this, Action<object?> callback, object? state)
	{
		var restoreFlow = false;
		if (!ExecutionContext.IsFlowSuppressed())
		{
			ExecutionContext.SuppressFlow();
			restoreFlow = true;
		}

		try
		{
			return @this.Register(callback, state, false);
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


