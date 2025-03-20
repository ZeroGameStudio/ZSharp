// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public interface IZeroStreamEnumerator<out T>
{
	ZeroTask DisposeAsync();
	ZeroTask<bool> MoveNextAsync();
	T Current { get; }
}



