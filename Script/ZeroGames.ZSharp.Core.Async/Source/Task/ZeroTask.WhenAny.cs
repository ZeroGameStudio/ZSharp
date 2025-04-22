// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.Async;

partial struct ZeroTask
{

	public static ZeroTask WhenAny(params IEnumerable<ZeroTask> tasks) => throw new NotImplementedException();
	public static ZeroTask<T> WhenAny<T>(params IEnumerable<ZeroTask<T>> tasks) => throw new NotImplementedException();

}


