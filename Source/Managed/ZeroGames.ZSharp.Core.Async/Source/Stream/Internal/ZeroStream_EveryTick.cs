// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

internal sealed class ZeroStream_EveryTick : IZeroStream<float>
{

	public ZeroStream_EveryTick(EEventLoopTickingGroup tickingGroup, Lifecycle lifecycle)
	{
		_tickingGroup = tickingGroup;
		_lifecycle = lifecycle;
	}

	public IZeroStreamEnumerator<float> GetAsyncEnumerator() => new Enumerator(_tickingGroup, _lifecycle);

	private class Enumerator : IZeroStreamEnumerator<float>
	{
		public ZeroTask DisposeAsync()
		{
			InternalDispose();
			GC.SuppressFinalize(this);
			return ZeroTask.CompletedTask;
		}

		public ZeroTask<bool> MoveNextAsync()
		{
			if (_disposed)
			{
				throw new InvalidOperationException();
			}

			if (_lifecycle.IsExpired)
			{
				return ZeroTask.FromResult(false);
			}

			return InternalMoveNextAsync();
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

		~Enumerator() => InternalDispose();

		private async ZeroTask<bool> InternalMoveNextAsync()
		{
			_current = await ZeroTask.Yield(_tickingGroup, _lifecycle);
			return true;
		}

		private void InternalDispose()
		{
			_disposed = true;
		}

		private readonly EEventLoopTickingGroup _tickingGroup;
		private readonly Lifecycle _lifecycle;

		private float _current;
		
		private bool _disposed;
	}
	
	private readonly EEventLoopTickingGroup _tickingGroup;
	private readonly Lifecycle _lifecycle;
	
}


