// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

internal sealed class ZeroStream_IntervalFrame : IZeroStream<int32>
{

	public ZeroStream_IntervalFrame(int32 intervalFrames, Lifetime lifetime)
	{
		_intervalFrames = intervalFrames;
		_lifetime = lifetime;
	}

	public IZeroStreamEnumerator<int32> GetAsyncEnumerator()
	{
		Thrower.ThrowIfNotInGameThread();
		return new Enumerator(_intervalFrames, _lifetime);
	}

	private class Enumerator : IZeroStreamEnumerator<int32>
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

			_current = await ZeroTask.DelayFrame(_intervalFrames, _lifetime);
			return true;
		}

		public int32 Current
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
		
		internal Enumerator(int32 intervalFrames, Lifetime lifetime)
		{
			_intervalFrames = intervalFrames;
			_lifetime = lifetime;
		}

		private readonly int32 _intervalFrames;
		private readonly Lifetime _lifetime;

		private int32 _current;
		
		private bool _disposed;
	}
	
	private readonly int32 _intervalFrames;
	private readonly Lifetime _lifetime;
	
}


