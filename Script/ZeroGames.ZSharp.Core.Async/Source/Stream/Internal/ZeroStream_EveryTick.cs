// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

internal sealed class ZeroStream_EveryTick : IZeroStream<float>
{

	public ZeroStream_EveryTick(EEventLoopTickingGroup tickingGroup, Lifetime lifetime)
	{
		_tickingGroup = tickingGroup;
		_lifetime = lifetime;
	}

	public IZeroStreamEnumerator<float> GetAsyncEnumerator()
	{
		Thrower.ThrowIfNotInGameThread();
		return new Enumerator(_tickingGroup, _lifetime);
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

			_current = await ZeroTask.Yield(_tickingGroup, _lifetime);
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
		
		internal Enumerator(EEventLoopTickingGroup tickingGroup, Lifetime lifetime)
		{
			_tickingGroup = tickingGroup;
			_lifetime = lifetime;
		}

		private readonly EEventLoopTickingGroup _tickingGroup;
		private readonly Lifetime _lifetime;

		private float _current;
		
		private bool _disposed;
	}
	
	private readonly EEventLoopTickingGroup _tickingGroup;
	private readonly Lifetime _lifetime;
	
}


