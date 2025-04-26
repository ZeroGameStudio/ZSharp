// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

partial struct ZeroTask
{

	public static IZeroStream<(T? Result, Exception? Exception)> WhenEach<T>(params IEnumerable<ZeroTask<T>> tasks)
		=> new ZeroStream_WhenEach<T>(tasks);

}


