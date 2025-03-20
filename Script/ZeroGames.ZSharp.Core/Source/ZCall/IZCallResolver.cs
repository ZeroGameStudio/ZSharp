// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

public interface IZCallResolver
{
	IZCallDispatcher? Resolve(string name);
}


