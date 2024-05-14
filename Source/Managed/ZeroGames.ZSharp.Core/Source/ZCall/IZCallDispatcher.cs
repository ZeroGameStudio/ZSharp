// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

public unsafe interface IZCallDispatcher
{
	int32 Dispatch(ZCallBuffer* buffer);
	string Name { get; }
}


