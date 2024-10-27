// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

internal sealed class ZCallDispatcher_Property(string name) : IZCallDispatcher
{
	
	public unsafe EZCallErrorCode Dispatch(ZCallBuffer* buffer)
	{
		throw new NotImplementedException();
	}

	public string Name { get; } = name;

}


