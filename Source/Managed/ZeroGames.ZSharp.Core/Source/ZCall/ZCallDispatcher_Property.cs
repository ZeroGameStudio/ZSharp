﻿// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

public class ZCallDispatcher_Property : IZCallDispatcher
{
	
	public unsafe int32 Dispatch(ZCallBuffer* buffer)
	{
		throw new NotImplementedException();
	}

	public required string Name { get; init; }
	
}

