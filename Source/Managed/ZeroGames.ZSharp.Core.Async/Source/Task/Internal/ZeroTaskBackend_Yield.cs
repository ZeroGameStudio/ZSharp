// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

internal class ZeroTaskBackend_Yield : PoolableZeroTaskBackendBase<float, ZeroTaskBackend_Yield>
{
	
	public static ZeroTaskBackend_Yield GetFromPool(EEventLoopTickingGroup tickingGroup, Lifecycle lifecycle)
	{
		var task = Pool.Pop();
		task._tickingGroup = tickingGroup;
		task.Lifecycle = lifecycle;

		return task;
	}

	public void Run()
	{
		_registration = IEventLoop.Instance.Register(_tickingGroup, static (in EventLoopArgs args, object? state, ref bool @continue) =>
		{
			var @this = (ZeroTaskBackend_Yield)state!;
			@this.SetResult(args.WorldDeltaSeconds);
			@continue = false;
		}, this, Lifecycle, (ex, state) =>
		{
			var @this = (ZeroTaskBackend_Yield)state!;
			@this.SetException(ex);
		});
	}

	protected override void Deinitialize()
	{
		_tickingGroup = default;
		_registration = default;
		
		base.Deinitialize();
	}

	private EEventLoopTickingGroup _tickingGroup;
	
	private EventLoopRegistration _registration;
	
}


