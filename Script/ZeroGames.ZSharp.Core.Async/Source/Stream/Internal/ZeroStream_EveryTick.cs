// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

internal sealed class ZeroStream_EveryTick : IZeroStream<float>
{

	public ZeroStream_EveryTick(EEventLoopTickingGroup tickingGroup, Lifecycle lifecycle)
	{
		_tickingGroup = tickingGroup;
		_lifecycle = lifecycle;
	}

	public IZeroStreamEnumerator<float> GetAsyncEnumerator()
	{
		Thrower.ThrowIfNotInGameThread();
		return new Enumerator(_tickingGroup, _lifecycle);
	}

	private class Enumerator : IZeroStreamEnumerator<float>
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

			_current = await ZeroTask.Yield(_tickingGroup, _lifecycle);
			return true;
		}

		public float Current
		{
			get
			{
				if (_disposed)
				{
					throw new InvalidOperationException();
				}

				return _current;
			}
		}
		
		internal Enumerator(EEventLoopTickingGroup tickingGroup, Lifecycle lifecycle)
		{
			_tickingGroup = tickingGroup;
			_lifecycle = lifecycle;
		}

		private readonly EEventLoopTickingGroup _tickingGroup;
		private readonly Lifecycle _lifecycle;

		private float _current;
		
		private bool _disposed;
	}
	
	private readonly EEventLoopTickingGroup _tickingGroup;
	private readonly Lifecycle _lifecycle;
	
}


