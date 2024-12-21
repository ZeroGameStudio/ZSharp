// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;

namespace ZeroGames.ZSharp.Core.Async;

internal class ZeroTask_Yield : UnderlyingZeroTaskBase<float, ZeroTask_Yield>
{
	
	public static ZeroTask_Yield GetFromPool(EEventLoopTickingGroup tickingGroup, Lifecycle lifecycle)
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
			var @this = Unsafe.As<ZeroTask_Yield>(state!);
			@this.SetResult(args.WorldDeltaSeconds);
			@continue = false;
		}, this, Lifecycle, (ex, state) =>
		{
			var @this = Unsafe.As<ZeroTask_Yield>(state!);
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


