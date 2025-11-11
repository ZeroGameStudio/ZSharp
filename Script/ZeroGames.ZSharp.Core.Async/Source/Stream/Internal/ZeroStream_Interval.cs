// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

internal sealed class ZeroStream_Interval : IZeroStream<float>
{

	public ZeroStream_Interval(EZeroTaskDelayType delayType, float intervalSeconds, Lifetime lifetime)
	{
		_delayType = delayType;
		_intervalSeconds = intervalSeconds;
		_lifetime = lifetime;
	}

	public IZeroStreamEnumerator<float> GetAsyncEnumerator()
	{
		Thrower.ThrowIfNotInGameThread();
		return new Enumerator(_delayType, _intervalSeconds, _lifetime);
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

			_current = await ZeroTask.Delay(_delayType, _intervalSeconds, _lifetime);
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
		
		internal Enumerator(EZeroTaskDelayType delayType, float intervalSeconds, Lifetime lifetime)
		{
			_delayType = delayType;
			_intervalSeconds = intervalSeconds;
			_lifetime = lifetime;
		}

		private readonly EZeroTaskDelayType _delayType;
		private readonly float _intervalSeconds;
		private readonly Lifetime _lifetime;

		private float _current;
		
		private bool _disposed;
	}
	
	private readonly EZeroTaskDelayType _delayType;
	private readonly float _intervalSeconds;
	private readonly Lifetime _lifetime;
	
}


