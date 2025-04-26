// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

partial struct ZeroTask
{

	public static ZeroTask WhenAll(params IEnumerable<ZeroTask> tasks) => new(new ZeroTaskBackend_WhenAll<AsyncVoid>(tasks.Select(task => (ZeroTask<AsyncVoid>)task)));
	public static ZeroTask<T[]> WhenAll<T>(params IEnumerable<ZeroTask<T>> tasks) => new(new ZeroTaskBackend_WhenAll<T>(tasks));

}


