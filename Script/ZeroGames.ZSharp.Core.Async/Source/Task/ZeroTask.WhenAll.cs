// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

partial struct ZeroTask
{

	public static ZeroTask WhenAll(params IEnumerable<ZeroTask> tasks) => throw new NotImplementedException();
	public static ZeroTask<T[]> WhenAll<T>(IEnumerable<ZeroTask<T>> tasks) => throw new NotImplementedException();

}


