// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

public unsafe interface IZCallDispatcher
{
	EZCallErrorCode Dispatch(ZCallBuffer* buffer);
	string Name { get; }
}


