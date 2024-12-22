// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;

namespace ZeroGames.ZSharp.Core.Async;

internal class ZeroTaskBackend_DelayFrames : ZeroTaskBackendBase<int32, ZeroTaskBackend_DelayFrames>
{

	public static ZeroTaskBackend_DelayFrames GetFromPool(int32 delayFrames, Lifecycle lifecycle, IProgress<int32>? progress)
	{
		var task = Pool.Pop();
		task._delayFrames = delayFrames;
		task.Lifecycle = lifecycle;

		return task;
	}

	public void Run()
	{
		_timer = GlobalTimerSchedulers.Frame.Register(static (deltaTime, state) =>
		{
			var @this = Unsafe.As<ZeroTaskBackend_DelayFrames>(state!);
			@this.SetResult(deltaTime);
		}, this, _delayFrames, false, false, Lifecycle, static (ex, state) =>
		{
			var @this = Unsafe.As<ZeroTaskBackend_DelayFrames>(state!);
			@this.SetException(ex);
		});
	}

	protected override void Deinitialize()
	{
		_delayFrames = 0;
		_timer = default;
		
		base.Deinitialize();
	}

	private int32 _delayFrames;
	
	private Timer _timer;

}


