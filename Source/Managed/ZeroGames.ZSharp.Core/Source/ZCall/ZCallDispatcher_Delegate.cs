// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

internal class ZCallDispatcher_Delegate : IZCallDispatcher
{

	public const string KName = "d://";
	
	public unsafe int32 Dispatch(ZCallBuffer* buffer)
	{
		throw new NotImplementedException();
	}

	public string Name => KName;

}


