// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

partial struct ZeroTask
{

	public static ZeroTask WhenAny(params IEnumerable<ZeroTask> tasks) => new(new ZeroTaskBackend_WhenAny<AsyncVoid>(tasks.Select(task => (ZeroTask<AsyncVoid>)task)));
	public static ZeroTask<(int32 WinnerIndex, T Result)> WhenAny<T>(params IEnumerable<ZeroTask<T>> tasks) => new(new ZeroTaskBackend_WhenAny<T>(tasks));

}


