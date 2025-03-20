// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

public interface IZeroStream<out T>
{
	IZeroStreamEnumerator<T> GetAsyncEnumerator();
}


