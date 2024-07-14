// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

internal class ZCallDispatcher_Delegate : IZCallDispatcher
{

	public const string KName = "d://";
	
	public unsafe int32 Dispatch(ZCallBuffer* buffer)
	{
		Delegate? @delegate = (*buffer)[0].GCHandle.Target as Delegate;
		if (@delegate is null)
		{
			return 1;
		}

		@delegate.DynamicInvoke();
		return 0;
	}

	public string Name => KName;

}


